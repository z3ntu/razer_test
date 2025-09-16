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

#include "razerfakedevice.h"

#include "../led/razerfakeled.h"

int RazerFakeDevice::serialCounter = 1000;

bool RazerFakeDevice::openDeviceHandle()
{
    serial = QString("FAKE%1").arg(serialCounter++);
    return true;
}

bool RazerFakeDevice::initialize()
{
    foreach (openrazer::LedId ledId, ledIds) {
        auto *rled = new RazerFakeLED(this, ledId);
        if (!rled->initialize()) {
            qWarning("Error while initializing LED with ID '%hhu'", static_cast<uchar>(ledId));
            delete rled;
            return false;
        }
        leds.insert(ledId, rled);
    }
    return true;
}

/* --------------------- DBUS METHODS --------------------- */

QString RazerFakeDevice::getSerial()
{
    qDebug("Called %s", Q_FUNC_INFO);
    return serial;
}

QString RazerFakeDevice::getFirmwareVersion()
{
    qDebug("Called %s", Q_FUNC_INFO);
    return fwVersion;
}

QString RazerFakeDevice::getKeyboardLayout()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("keyboard_layout"))
        return "error";

    return keyboardLayoutIds.value(0x01, "unknown"); // en_US
}

openrazer::DPI RazerFakeDevice::getDPI()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("dpi"))
        return { 0, 0 };

    return dpi;
}

bool RazerFakeDevice::setDPI(openrazer::DPI dpi)
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("dpi"))
        return false;

    this->dpi = dpi;

    return true;
}

ushort RazerFakeDevice::getPollRate()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("poll_rate"))
        return 0;

    return poll_rate;
}

bool RazerFakeDevice::setPollRate(ushort poll_rate)
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("poll_rate"))
        return false;

    if (poll_rate == 1000 || poll_rate == 500 || poll_rate == 125) {
        this->poll_rate = poll_rate;
    } else {
        dbusFailedHelper("Invalid poll rate.");
        return false;
    }
    return true;
}

bool RazerFakeDevice::displayCustomFrame()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("custom_frame"))
        return false;
    return true;
}

bool RazerFakeDevice::defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QVector<openrazer::RGB> rgbData)
{
    qDebug("Called %s with param %i, %i, %i", Q_FUNC_INFO, row, startColumn, endColumn);
    qDebug() << " (cont.) rgbData:" << rgbData;
    if (!checkFeature("custom_frame"))
        return false;

    if (rgbData.size() != (endColumn + 1 - startColumn)) {
        dbusFailedHelper(QString("defineCustomFrame called with invalid size (%d instead of %d) of rgbData").arg(rgbData.size(), (endColumn + 1 - startColumn)));
        return false;
    }
    return true;
}
