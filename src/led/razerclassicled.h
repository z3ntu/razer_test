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

#ifndef RAZERCLASSICLED_H
#define RAZERCLASSICLED_H

#include "razerled.h"

enum RazerClassicLedState {
    Off = 0x00,
    On = 0x01
};

enum RazerClassicEffectId {
    Static = 0x00,
    Blinking = 0x01,
    Pulsating = 0x02, // Breathing
    Spectrum = 0x04
};

/**
 * @todo write docs
 */
class RazerClassicLED : public RazerLED
{
public:
    RazerClassicLedState state;
    RazerClassicEffectId effect;
};

#endif // RAZERCLASSICLED_H
