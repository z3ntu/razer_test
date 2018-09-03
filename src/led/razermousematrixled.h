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

#ifndef RAZERMOUSEMATRIXLED_H
#define RAZERMOUSEMATRIXLED_H

#include "razerled.h"

enum class RazerMouseMatrixEffectId : unsigned char {
    Off          = 0x00,
    Static       = 0x01,
    Breathing    = 0x02,
    Spectrum     = 0x03,
    Wave         = 0x04,
    Reactive     = 0x05,
    Starlight    = 0x07,
    CustomFrame  = 0x08
};

/**
 * @todo write docs
 */
class RazerMouseMatrixLED : public RazerLED
{
public:
    using RazerLED::RazerLED;

    RazerMouseMatrixEffectId effect;
};

#endif // RAZERMOUSEMATRIXLED_H
