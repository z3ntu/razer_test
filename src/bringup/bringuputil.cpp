/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018-2019  Luca Weiss <luca@z3ntu.xyz>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bringuputil.h"

#include "../device/razerclassicdevice.h"
#include "../device/razermatrixdevice.h"
#include "../validjsonvalues.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <iostream>

BringupUtil::BringupUtil(struct hid_device_info *hid_dev_info)
    : hid_dev_info(hid_dev_info)
{
}

BringupUtil::BringupUtil(RazerDevice *device)
    : device(device)
{
}

const QVector<QVector<RazerDeviceQuirks>> quirksCombinations {
    {},
    { RazerDeviceQuirks::MouseMatrix },
    { RazerDeviceQuirks::MouseMatrix, RazerDeviceQuirks::MatrixBrightness },
    { RazerDeviceQuirks::MatrixBrightness }
};

RazerDevice *BringupUtil::tryDevice(QString pclass, QVector<openrazer::LedId> ledIds, QStringList fx, QVector<RazerDeviceQuirks> quirks)
{
    RazerDevice *device;
    if (pclass == "classic") {
        device = new RazerClassicDevice(hid_dev_info->path, hid_dev_info->vendor_id, hid_dev_info->product_id, /*name*/ "", /*type*/ "", ledIds, fx, validFeatures, quirks, /*dims*/ {}, /*maxDPI*/ 0);
    } else if (pclass == "matrix") {
        device = new RazerMatrixDevice(hid_dev_info->path, hid_dev_info->vendor_id, hid_dev_info->product_id, /*name*/ "", /*type*/ "", ledIds, fx, validFeatures, quirks, /*dims*/ {}, /*maxDPI*/ 0);
    } else {
        qCritical("Unhandled pclass in BringupUtil::tryDevice");
        return nullptr;
    }

    if (!device->openDeviceHandle()) {
        qCritical("Failed to open device handle.");
        qCritical("You can give your current user permissions to access the hidraw nodes with the following commands:");
        qCritical("$ sudo chmod g+rw /dev/hidraw*");
        qCritical("$ sudo chgrp $(id -g) /dev/hidraw*");
        qFatal("Exiting now.");
    }
    if (!device->initialize()) {
        qWarning("Failed to initialize device.");
        delete device;
        return nullptr;
    }
    qDebug("Successfully initialized LEDs.");
    return device;
}

bool BringupUtil::newDevice()
{
    QTextStream cin(stdin);

    QString input;
    bool inputValid = true;

    QString name;
    QString vid;
    QString pid;
    QString type;

    qInfo("=============================");
    qInfo("== razer_test bringup util ==");
    qInfo("==    NEW DEVICE WIZARD    ==");
    qInfo("=============================");
    name = QString::fromWCharArray(hid_dev_info->product_string);
    qInfo("Product: %s", qUtf8Printable(name));
    vid = QString::number(hid_dev_info->vendor_id, 16).rightJustified(4, '0');
    qInfo("VID: %s", qUtf8Printable(vid));
    pid = QString::number(hid_dev_info->product_id, 16).rightJustified(4, '0');
    qInfo("PID: %s", qUtf8Printable(pid));
    qInfo("Do you want to bring up this new device? (y/N)");
    std::cout << "> ";
    input = cin.readLine();
    if (input.compare("y", Qt::CaseInsensitive) != 0) {
        qInfo("Exiting bringup util.");
        return true;
    }

    QVector<openrazer::LedId> ledIds = {};
    QStringList features = {};
    openrazer::MatrixDimensions dims = {};
    ushort maxDPI = 0;
    bool brightnessWorks = false;

    QString pclass;
    QVector<RazerDeviceQuirks> quirks;

    for (const QString &pclass_try : allPclasses) {
        for (QVector<RazerDeviceQuirks> quirks_try : quirksCombinations) {
            QString quirksStr = "[";
            for (RazerDeviceQuirks quirk : quirks_try)
                quirksStr.append(QuirksToString.value(quirk) + ",");
            quirksStr.append("]");
            qDebug("Trying pclass %s and quirks %s", qUtf8Printable(pclass_try), qUtf8Printable(quirksStr));
            // "classic" devices can have no rgb and no brightness
            if (pclass_try == "classic") {
                device = tryDevice(pclass_try, allLedIds, onOffFx, quirks_try);
                if (device == nullptr)
                    continue;

                RazerDevice *rgbDevice = tryDevice(pclass_try, allLedIds, rgbFx, quirks_try);
                if (rgbDevice == nullptr) {
                    qInfo("Device seems incapable of RGB effects, ignore the warnings above.");
                    deviceHasRgb = false;
                } else {
                    delete device;
                    device = rgbDevice;
                    deviceHasRgb = true;
                }
            } else {
                device = tryDevice(pclass_try, allLedIds, rgbFx, quirks_try);
                if (device == nullptr)
                    continue;

                deviceHasRgb = true;
            }

            for (auto led : device->getLeds()) {
                led->setOff();
            }
            qInfo("Now all LEDs should be off. Is that correct? (y/N)");
            std::cout << "> ";
            input = cin.readLine();
            if (input.compare("y", Qt::CaseInsensitive) != 0) {
                qWarning("Testing out other pclass / quirks then...");
                delete device;
                continue;
            }

            // Save the values that have worked
            pclass = pclass_try;
            quirks = quirks_try;

            // Exit the loop
            goto device_valid;
        }
    }
    if (device == nullptr) {
        qCritical("Tried all quirks and pclasses and none worked. Exiting");
        return false;
    }

device_valid:
    // Try setStatic if the device has RGB, otherwise setOn
    for (auto led : device->getLeds()) {
        const char *effectDesc;
        if (deviceHasRgb) {
            led->setStatic({ 0xFF, 0xFF, 0x00 });
            effectDesc = "yellow";
        } else {
            led->setOn();
            effectDesc = "on";
        }
        qInfo("Did a LED just turn %s (tried %s)? (y/N)", effectDesc, qUtf8Printable(LedIdToString.value(led->getLedId())));
        std::cout << "> ";
        input = cin.readLine();
        if (input.compare("y", Qt::CaseInsensitive) == 0) {
            ledIds.append(led->getLedId());
        }
        led->setOff();
    }

    if (ledIds.isEmpty()) {
        qInfo("You said that no LED is supported. Exiting. TODO: quirks"); // TODO
        return false;
    } else if (ledIds.size() == allLedIds.size()) {
        qWarning("You said that all LEDs do something. This is most likely not true; taking this as just the 'backlight' LED is supported.");
        ledIds = { openrazer::LedId::BacklightLED };
    }

    // == TYPE ==
    do {
        qInfo("What is your device type?");
        qInfo("Valid types: ");
        for (auto value : validType)
            qInfo("- %s", qUtf8Printable(value));
        std::cout << "> ";
        type = cin.readLine();
        if (!validType.contains(type)) {
            qCritical("Invalid device type.");
            inputValid = false;
        } else {
            inputValid = true;
        }
    } while (!inputValid);

    // Reinitialize device with figured-out ledIds
    // Get set FX to deduplicate the logic above with non-RGB vs RGB devices
    QStringList fxInUse = device->getSupportedFx();
    delete device;
    device = tryDevice(pclass, ledIds, fxInUse, quirks);
    if (device == nullptr) {
        qCritical("Re-initialized device with new Led IDs and failed.");
        qFatal("Exiting now.");
    }

    // == FX ==
    QVector<openrazer::Effect> fxVec = testLedEffects(device);

    brightnessWorks = device->hasFx("brightness") && testBrightness(device);

    // == FEATURES ==
    if (type == "mouse") {
        // == MAX DPI ==
        QString maxDPIstr;
        do {
            qInfo("What's the maximum DPI of the mouse (e.g. 16000)?");
            std::cout << "> ";
            maxDPIstr = cin.readLine();
            maxDPI = maxDPIstr.toUInt(nullptr, 10);
            if (maxDPI == 0) {
                qCritical("Invalid DPI.");
                inputValid = false;
            } else {
                inputValid = true;
            }
        } while (!inputValid);

        // == DPI ==
        if (testDPI(device)) {
            features.append("dpi");
        } else {
            // Try the ByteDPI quirk
            qInfo("Normal DPI check failed, trying ByteDPI quirk...");
            QVector<RazerDeviceQuirks> quirksNew = QVector<RazerDeviceQuirks>(quirks);
            quirksNew.append(RazerDeviceQuirks::ByteDPI);
            RazerDevice *deviceNew = tryDevice(pclass, ledIds, fxInUse, quirksNew);
            if (testDPI(deviceNew)) {
                delete device;
                device = deviceNew;
                quirks = quirksNew;
                features.append("dpi");
            } else {
                delete deviceNew;
                qWarning("Your device is a mouse but seems to not support DPI!");
            }
        }

        // == POLL RATE ==
        if (testPollRate(device)) {
            features.append("poll_rate");
        } else {
            qWarning("Your device is a mouse but seems to not support Poll Rate!");
        }
    }

    if (type == "keyboard") {
        // == KEYBOARD LAYOUT ==
        if (testKeyboardLayout(device))
            features.append("keyboard_layout");
    }

    // TODO: Custom frame

    // Convert to QJsonArray
    QJsonArray ledIdsJson;
    for (auto ledId : ledIds)
        ledIdsJson.append(static_cast<uchar>(ledId));
    QJsonArray fxJson;
    for (auto fx : fxVec)
        fxJson.append(EffectToString.value(fx));
    if (brightnessWorks)
        fxJson.append("brightness");
    QJsonArray featuresJson = QJsonArray::fromStringList(features);
    QJsonArray quirksJson;
    for (auto quirk : quirks)
        quirksJson.append(QuirksToString.value(quirk));
    QJsonArray dimsJson = { dims.x, dims.y };

    QJsonObject deviceObj {
        { "name", name },
        { "vid", vid },
        { "pid", pid },
        { "type", type },
        { "pclass", pclass },
        { "leds", ledIdsJson },
        { "fx", fxJson },
        { "features", featuresJson },
        { "quirks", quirksJson },
        { "matrix_dimensions", dimsJson }, // TODO
        { "max_dpi", maxDPI }
    };

    // Remove unnecessary entries
    if (type != "mouse")
        deviceObj.remove("max_dpi");
    if (quirksJson.isEmpty())
        deviceObj.remove("quirks");

    QJsonDocument doc(deviceObj);
    QString jsonString = doc.toJson(QJsonDocument::Indented);
    qInfo("JSON Result:\n\n%s", qUtf8Printable(jsonString));
    qInfo("You can copy this into ./data/devices/%s.json and run ./scripts/format-devices-json.py to format the file!", qUtf8Printable(type));

    return true;
}

bool BringupUtil::existingDevice()
{
    qInfo("=============================");
    qInfo("== razer_test bringup util ==");
    qInfo("==  DEVICE TESTING WIZARD  ==");
    qInfo("=============================");
    qInfo("Device: %s", qUtf8Printable(device->getName()));
    qInfo("Serial: %s", qUtf8Printable(device->getSerial()));
    qInfo("Firmware version: %s", qUtf8Printable(device->getFirmwareVersion()));

    QTextStream cin(stdin);
    QString input;

    qInfo("Do you want to test this device? (y/N)");
    std::cout << "> ";
    input = cin.readLine();
    if (input.compare("y", Qt::CaseInsensitive) != 0) {
        qInfo("Exiting bringup util.");
        return false;
    }

    foreach (RazerLED *led, device->getLeds()) {
        qInfo("Setting LED to static with color #FFFF00");
        qDebug("LED object path: %s", qUtf8Printable(led->getObjectPath().path()));
        led->setStatic({ 0xFF, 0xFF, 0x00 });
        led->setBrightness(255);
    }

    // Automatic tests
    if (!testDPI(device))
        return false;
    if (!testPollRate(device))
        return false;
    if (!testKeyboardLayout(device))
        return false;
    if (!testBrightness(device))
        return false;

    // Interactive tests
    // TODO: Check return value of testLedEffects
    testLedEffects(device);

    return true;
}

bool BringupUtil::testDPI(RazerDevice *device)
{
    if (!device->hasFeature("dpi")) {
        qDebug("Skip testDPI because the device doesn't have the feature.");
        return true; // unsupported
    }
    auto dpi = device->getDPI();
    if (dpi.dpi_x == 0 && dpi.dpi_y == 0) {
        qInfo("Failed to get current DPI.");
        return false;
    }
    qInfo("The current DPI is: %hu - %hu", dpi.dpi_x, dpi.dpi_y);
    openrazer::DPI newDPI = { 500, 600 };
    device->setDPI(newDPI);
    auto dpi2 = device->getDPI();
    if (dpi2.dpi_x != newDPI.dpi_x || dpi2.dpi_y != newDPI.dpi_y) {
        qInfo("Failed setting DPI. A: %hu - %hu, B: %hu - %hu", newDPI.dpi_x, newDPI.dpi_y, dpi2.dpi_x, dpi2.dpi_y);
        return false;
    }
    // Reset DPI back
    device->setDPI(dpi);
    return true;
}

bool BringupUtil::testPollRate(RazerDevice *device)
{
    if (!device->hasFeature("poll_rate")) {
        qDebug("Skip testPollRate because the device doesn't have the feature.");
        return true; // unsupported
    }
    auto pollRate = device->getPollRate();
    if (pollRate == 0) {
        qInfo("Failed to get current poll rate.");
        return false;
    }
    qInfo("The current poll rate is: %hu", pollRate);
    ushort newPollRate;
    if (pollRate == 1000)
        newPollRate = 500;
    else
        newPollRate = 1000;
    device->setPollRate(newPollRate);
    auto pollRate2 = device->getPollRate();
    if (pollRate2 != newPollRate) {
        qInfo("Failed setting poll rate. A: %hu, B: %hu", newPollRate, pollRate2);
        return false;
    }
    // Reset poll rate back
    device->setPollRate(pollRate);
    return true;
}

bool BringupUtil::testBrightness(RazerDevice *device)
{
    if (!device->hasFx("brightness")) {
        qDebug("Skip testBrightness because the device doesn't have the feature.");
        return true; // unsupported
    }
    for (auto led : device->getLeds()) {
        auto brightness = led->getBrightness();
        qInfo("The current brightness on LED %s is: %hhu", qUtf8Printable(LedIdToString.value(led->getLedId())), brightness);
        uchar newBrightness;
        if (brightness > 0x80)
            newBrightness = 0x40;
        else
            newBrightness = 0xFF;
        led->setBrightness(newBrightness);
        auto brightness2 = led->getBrightness();
        if (brightness2 != newBrightness) {
            qInfo("Failed setting brightness. A: %hhu, B: %hhu", newBrightness, brightness2);
            return false;
        }
        // Reset poll rate back
        led->setBrightness(brightness);
    }
    return true;
}

bool BringupUtil::testKeyboardLayout(RazerDevice *device)
{
    if (!device->hasFeature("keyboard_layout")) {
        qDebug("Skip testKeyboardLayout because the device doesn't have the feature.");
        return true; // unsupported
    }
    auto layout = device->getKeyboardLayout();
    if (layout == "error") {
        qInfo("Failed to determine keyboard layout.");
        return false;
    } else if (layout == "unknown") {
        qInfo("Failed to determine keyboard layout. Either yours is unknown (you can get the ID output when you use the --verbose parameter) or your device doesn't support this call.");
        return false;
    } else {
        qInfo("Your keyboard layout is: %s", qUtf8Printable(layout));
        return true;
    }
}

QVector<openrazer::Effect> BringupUtil::testLedEffects(RazerDevice *device)
{
    QTextStream cin(stdin);
    QString input;

    QVector<openrazer::Effect> workingEffects;

    for (auto led : device->getLeds()) {
        qInfo("-- Checking functionality of LED '%s' --", qUtf8Printable(LedIdToString.value(led->getLedId())));
        auto currEffect = led->getCurrentEffect();
        auto currColors = led->getCurrentColors();
        // LED-local array
        QVector<openrazer::Effect> workingEffectsLed;
        for (auto effectStr : device->getSupportedFx()) {
            // Skip brightness "effect" here
            if (effectStr == "brightness")
                continue;
            // Try to set the effect (setEffect can also return false for unimplemented effects, e.g. blinking on Matrix)
            openrazer::Effect effect = StringToEffect.value(effectStr);
            if (!setEffect(led, effect, { 255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 })) {
                qDebug("Setting effect %s returned false, skipping...", qUtf8Printable(effectStr));
                continue;
            }
            // Ask the user if it worked
            qInfo("Effect %s should be active now. Is that correct? (y/N)", qUtf8Printable(effectStr));
            std::cout << "> ";
            input = cin.readLine();
            if (input.compare("y", Qt::CaseInsensitive) == 0) {
                workingEffectsLed.append(effect);
            }
        }
        if (workingEffects.size() == 0) {
            workingEffects = workingEffectsLed;
        } else {
            if (workingEffects.size() != workingEffectsLed.size()) {
                qWarning("You said, that this LED has a different amount of supported effects compared to the first one.");
                qWarning("If you're sure, this wasn't a mistake on your side, please open an issue in the repo!");
                qWarning("Ignoring this now...");
            }
        }

        if (currColors.size() <= 2) // should never happen
            currColors = { { 255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 } };
        // Restore effect
        setEffect(led, currEffect, currColors.at(0), currColors.at(1), currColors.at(2));
    }
    return workingEffects;
}

bool BringupUtil::setEffect(RazerLED *led, openrazer::Effect effect, openrazer::RGB color1, openrazer::RGB color2, openrazer::RGB color3)
{
    Q_UNUSED(color3)
    if (effect == openrazer::Effect::Off) {
        return led->setOff();
    } else if (effect == openrazer::Effect::On) {
        return led->setOn();
    } else if (effect == openrazer::Effect::Static) {
        return led->setStatic(color1);
    } else if (effect == openrazer::Effect::Blinking) {
        return led->setBlinking(color1);
    } else if (effect == openrazer::Effect::Breathing) {
        return led->setBreathing(color1);
    } else if (effect == openrazer::Effect::BreathingDual) {
        return led->setBreathingDual(color1, color2);
    } else if (effect == openrazer::Effect::BreathingRandom) {
        return led->setBreathingRandom();
    } else if (effect == openrazer::Effect::Spectrum) {
        return led->setSpectrum();
    } else if (effect == openrazer::Effect::Wave) {
        return led->setWave(openrazer::WaveDirection::LEFT_TO_RIGHT);
    } else if (effect == openrazer::Effect::Reactive) {
        return led->setReactive(openrazer::ReactiveSpeed::_1000MS, color1);
    } else {
        qCritical("Unhandled effect %s at %s", qUtf8Printable(EffectToString.value(effect)), Q_FUNC_INFO);
        return false;
    }
}
