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

#ifndef BRINGUPUTIL_H
#define BRINGUPUTIL_H

#include "../device/razerdevice.h"

#include <hidapi.h>

/**
 * @todo write docs
 */
class BringupUtil
{
public:
    BringupUtil(struct hid_device_info *hid_dev_info);
    BringupUtil(RazerDevice *device);
    bool newDevice();
    bool existingDevice();

    // Automatic
    static bool testDPI(RazerDevice *device);
    static bool testPollRate(RazerDevice *device);
    static bool testKeyboardLayout(RazerDevice *device);
    static bool testBrightness(RazerDevice *device);

    // Interactive
    static QVector<RazerEffect> testLedEffects(RazerDevice *device);

private:
    struct hid_device_info *hid_dev_info;
    RazerDevice *device;
    bool deviceHasRgb;

    static bool setEffect(RazerLED *led, razer_test::RazerEffect effect, razer_test::RGB color1, razer_test::RGB color2, razer_test::RGB color3);
    RazerDevice *tryDevice(QString pclass, QVector<razer_test::RazerLedId> ledIds, QStringList fx, QVector<RazerDeviceQuirks> quirks);

    // Removed MacroRecordingLED, GameModeLED, KeymapRedLED, KeymapGreenLED, KeymapBlueLED
    const QVector<RazerLedId> allLedIds = { RazerLedId::ScrollWheelLED, RazerLedId::BatteryLED, RazerLedId::LogoLED, RazerLedId::BacklightLED, RazerLedId::RightSideLED, RazerLedId::LeftSideLED };
    // The case for super old devices, that just support "LED on" and "LED off", no brightness, no color
    const QStringList onOffFx = { "off", "on" };
    const QStringList rgbFx = { "off", "static", "blinking", "breathing", "breathing_dual", "breathing_random", "spectrum", "wave", "reactive", "brightness" };
    const QStringList allPclasses = { "matrix", "classic" };
};

#endif // BRINGUPUTIL_H
