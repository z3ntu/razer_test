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

#ifndef RAZERMATRIXLED_H
#define RAZERMATRIXLED_H

#include "razerled.h"
#include "razermousematrixled.h" // TODO What to do here?? Merge them?

/**
 * @todo write docs
 */
class RazerMatrixLED : public RazerLED
{
public:
    using RazerLED::RazerLED;

    RazerMatrixEffectId effect;

    virtual bool setNone(RazerLedId led);
    virtual bool setStatic(RazerLedId led, uchar red, uchar green, uchar blue);
    virtual bool setBreathing(RazerLedId led, uchar red, uchar green, uchar blue);
    virtual bool setBreathingDual(RazerLedId led, uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2);
    virtual bool setBreathingRandom(RazerLedId led);
    virtual bool setBlinking(RazerLedId led, uchar red, uchar green, uchar blue);
    virtual bool setSpectrum(RazerLedId led);
    virtual bool setWave(RazerLedId led, WaveDirection direction);
    virtual bool setReactive(RazerLedId led, ReactiveSpeed speed, uchar red, uchar green, uchar blue);

    virtual bool setBrightness(RazerLedId led, uchar brightness);
    virtual bool getBrightness(RazerLedId led, uchar *brightness);

    bool setSpectrumInit(RazerLedId led);

private:
    bool setMatrixEffect(RazerLedId led, RazerMatrixEffectId effect,
                         uchar arg1 = 0x00, uchar arg2 = 0x00, uchar arg3 = 0x00, uchar arg4 = 0x00, uchar arg5 = 0x00, uchar arg6 = 0x00, uchar arg7 = 0x00, uchar arg8 = 0x00);
    bool setMouseMatrixEffect(RazerLedId led, RazerMouseMatrixEffectId effect,
                              uchar arg3 = 0x00, uchar arg4 = 0x00, uchar arg5 = 0x00, uchar arg6 = 0x00, uchar arg7 = 0x00, uchar arg8 = 0x00, uchar arg9 = 0x00, uchar arg10 = 0x00, uchar arg11 = 0x00);
};

#endif // RAZERMATRIXLED_H
