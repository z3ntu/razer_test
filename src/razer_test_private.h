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

#ifndef RAZERTESTPRIVATE_H
#define RAZERTESTPRIVATE_H

#include <QtGlobal>

enum class RazerVarstore : uchar {
    NOSTORE = 0x00,
    STORE = 0x01
};

// TODO: Convert to enum class
enum RazerStatus {
    NEW = 0x00,
    BUSY = 0x01,
    SUCCESSFUL = 0x02,
    FAILURE = 0x03,
    NO_RESPONSE_TIMEOUT = 0x04,
    NOT_SUPPORTED = 0x05
};

enum class RazerClassicLedState : uchar {
    Off = 0x00,
    On = 0x01
};

enum class RazerClassicEffectId : uchar {
    Static = 0x00,
    Blinking = 0x01,
    Breathing = 0x02, // Pulsating
    Spectrum = 0x04
};

enum class RazerMatrixEffectId : uchar {
    Off         = 0x00,
    Wave        = 0x01,
    Reactive    = 0x02, // Afterglow
    Breathing   = 0x03,
    Spectrum    = 0x04,
    CustomFrame = 0x05,
    Static      = 0x06,
    Starlight   = 0x19
};

enum class RazerMouseMatrixEffectId : uchar {
    Off          = 0x00,
    Static       = 0x01,
    Breathing    = 0x02,
    Spectrum     = 0x03,
    Wave         = 0x04,
    Reactive     = 0x05,
    Starlight    = 0x07,
    CustomFrame  = 0x08
};

enum class RazerDeviceQuirks {
    MouseMatrix,
    MatrixBrightness
};

#endif // RAZERTESTPRIVATE_H
