/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018  Luca Weiss <luca@z3ntu.xyz>
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

int RazerFakeDevice::serialCounter = 1000;

bool RazerFakeDevice::openDeviceHandle()
{
    serial = QString("FAKE%1").arg(serialCounter++);
    fwVersion = "v99.99";
    return true;
}

bool RazerFakeDevice::initializeLeds()
{
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
    return keyboardLayoutIds.value(0x01, "unknown"); // en_US
}

bool RazerFakeDevice::setNone(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if (!checkLedAndFx(led, "off"))
        return false;
    return true;
}

bool RazerFakeDevice::setStatic(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue);
    if (!checkLedAndFx(led, "static"))
        return false;
    return true;
}

bool RazerFakeDevice::setBreathing(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue);
    if (!checkLedAndFx(led, "breathing"))
        return false;
    return true;
}

bool RazerFakeDevice::setBreathingDual(RazerLedId led, uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2)
{
    qDebug("Called %s with params %hhu, %i, %i, %i, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue, red2, green2, blue2);
    if (!checkLedAndFx(led, "breathing_dual"))
        return false;
    return true;
}

bool RazerFakeDevice::setBreathingRandom(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if (!checkLedAndFx(led, "breathing_random"))
        return false;
    return true;
}

bool RazerFakeDevice::setBlinking(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue);
    if (!checkLedAndFx(led, "blinking"))
        return false;
    return true;
}

bool RazerFakeDevice::setSpectrum(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if (!checkLedAndFx(led, "spectrum"))
        return false;
    return true;
}

bool RazerFakeDevice::setWave(RazerLedId led, WaveDirection direction)
{
    qDebug("Called %s with params %hhu, %hhu", Q_FUNC_INFO, static_cast<uchar>(led), static_cast<uchar>(direction));
    if (!checkLedAndFx(led, "wave"))
        return false;
    return true;
}

bool RazerFakeDevice::setCustomFrame(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if (!checkLedAndFx(led, "custom_frame"))
        return false;
    return true;
}

bool RazerFakeDevice::setBrightness(RazerLedId led, uchar brightness)
{
    qDebug("Called %s with params %hhu, %i", Q_FUNC_INFO, static_cast<uchar>(led), brightness);
    if (!checkLedAndFx(led, QString::null))
        return false;
    return true;
}

bool RazerFakeDevice::getBrightness(RazerLedId led, uchar *brightness)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if (!checkLedAndFx(led, QString::null))
        return false;
    return 100;
}
