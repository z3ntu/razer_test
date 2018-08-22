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

bool RazerFakeDevice::initializeLeds()
{
    return true;
}

QString RazerFakeDevice::getSerial()
{
    return "FAKE1234";
}

QString RazerFakeDevice::getFirmwareVersion()
{
    return "v99.99";
}

bool RazerFakeDevice::setNone(RazerLedId led)
{
    return true;
}

bool RazerFakeDevice::setStatic(RazerLedId led, uchar red, uchar green, uchar blue)
{
    return true;
}

bool RazerFakeDevice::setBreathing(RazerLedId led, uchar red, uchar green, uchar blue)
{
    return true;
}

bool RazerFakeDevice::setBreathingDual(RazerLedId led, uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2)
{
    return true;
}

bool RazerFakeDevice::setBreathingRandom(RazerLedId led)
{
    return true;
}

bool RazerFakeDevice::setBlinking(RazerLedId led, uchar red, uchar green, uchar blue)
{
    return true;
}

bool RazerFakeDevice::setSpectrum(RazerLedId led)
{
    return true;
}

bool RazerFakeDevice::setWave(RazerLedId led, WaveDirection direction)
{
    return true;
}

bool RazerFakeDevice::setCustomFrame(RazerLedId led)
{
    return true;
}

bool RazerFakeDevice::setBrightness(RazerLedId led, uchar brightness)
{
    return true;
}

bool RazerFakeDevice::getBrightness(RazerLedId led, uchar *brightness)
{
    return 100;
}
