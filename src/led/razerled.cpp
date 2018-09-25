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

#include "razerled.h"

RazerLED::RazerLED(RazerDevice *device, RazerLedId ledId) : device(device), ledId(ledId)
{
}

RazerLED::~RazerLED()
    = default;

QDBusObjectPath RazerLED::getObjectPath()
{
    return QDBusObjectPath(QString("/io/github/openrazer1/devices/%1/led/%2").arg(device->getSerial()).arg(static_cast<uchar>(ledId)));
}

uchar RazerLED::getBrightness()
{
    // Wrapper as D-Bus can't (easily) handle pointers / multiple return values
    // TODO: Note: Apparently it can, I don't know how to properly implement it though.
    uchar brightness = 0x00;
    getBrightness(&brightness);
    return brightness;
}

RazerEffect RazerLED::getCurrentEffect()
{
    return effect;
}

QList<RGB> RazerLED::getCurrentColors()
{
    return {color1, color2, color3};
}

bool RazerLED::checkFx(QString fxStr)
{
    if (!device->hasFx(fxStr)) {
        if (calledFromDBus())
            sendErrorReply(QDBusError::NotSupported, "Unsupported FX.");
        return false;
    }
    return true;
}

void RazerLED::saveFxAndColors(RazerEffect fx, int numColors, RGB color1, RGB color2, RGB color3)
{
    this->effect = fx;
    if (numColors >= 1) {
        this->color1 = color1;
    }
    if (numColors >= 2) {
        this->color2 = color2;
    }
    if (numColors >= 3) {
        this->color3 = color3;
    }
}
