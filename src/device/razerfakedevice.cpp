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

QString RazerFakeDevice::getSerial()
{
    return serial;
}

QString RazerFakeDevice::getFirmwareVersion()
{
    return fwVersion;
}

bool RazerFakeDevice::setNone(RazerLedId led)
{
    qDebug("Called %s with param %i", Q_FUNC_INFO, led);
    return true;
}

bool RazerFakeDevice::setStatic(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %i, %i, %i, %i", Q_FUNC_INFO, led, red, green, blue);
    return true;
}

bool RazerFakeDevice::setBreathing(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %i, %i, %i, %i", Q_FUNC_INFO, led, red, green, blue);
    return true;
}

bool RazerFakeDevice::setBreathingDual(RazerLedId led, uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2)
{
    qDebug("Called %s with params %i, %i, %i, %i, %i, %i, %i", Q_FUNC_INFO, led, red, green, blue, red2, green2, blue2);
    return true;
}

bool RazerFakeDevice::setBreathingRandom(RazerLedId led)
{
    qDebug("Called %s with param %i", Q_FUNC_INFO, led);
    return true;
}

bool RazerFakeDevice::setBlinking(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %i, %i, %i, %i", Q_FUNC_INFO, led, red, green, blue);
    return true;
}

bool RazerFakeDevice::setSpectrum(RazerLedId led)
{
    qDebug("Called %s with param %i", Q_FUNC_INFO, led);
    return true;
}

bool RazerFakeDevice::setWave(RazerLedId led, WaveDirection direction)
{
    qDebug("Called %s with params %i, %hhu", Q_FUNC_INFO, led, direction);
    return true;
}

bool RazerFakeDevice::setCustomFrame(RazerLedId led)
{
    qDebug("Called %s with param %i", Q_FUNC_INFO, led);
    return true;
}

bool RazerFakeDevice::setBrightness(RazerLedId led, uchar brightness)
{
    qDebug("Called %s with params %i, %i", Q_FUNC_INFO, led, brightness);
    return true;
}

bool RazerFakeDevice::getBrightness(RazerLedId led, uchar *brightness)
{
    qDebug("Called %s with param %i", Q_FUNC_INFO, led);
    return 100;
}
