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

#ifndef RAZERLED_H
#define RAZERLED_H

enum RazerLedId {
    ScrollWheelLED = 0x01,
    BatteryLED = 0x03,
    LogoLED = 0x04,
    BacklightLED = 0x05,
    MacroRecordingLED = 0x07,
    GameModeLED = 0x08,
    KeymapRedLED = 0x0C,
    KeymapGreenLED = 0x0D,
    KeymapBlueLED = 0x0E
};

/**
 * @todo write docs
 */
class RazerLED
{
public:
    virtual ~RazerLED() = 0;
    RazerLedId ledId;
    unsigned char brightness;
    // current (classic) effect & current (classic) state in subclass
};

#endif // RAZERLED_H
