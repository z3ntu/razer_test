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

    virtual bool setNone();
    virtual bool setStatic(uchar red, uchar green, uchar blue);
    virtual bool setBreathing(uchar red, uchar green, uchar blue);
    virtual bool setBreathingDual(uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2);
    virtual bool setBreathingRandom();
    virtual bool setBlinking(uchar red, uchar green, uchar blue);
    virtual bool setSpectrum();
    virtual bool setWave(WaveDirection direction);
    virtual bool setReactive(ReactiveSpeed speed, uchar red, uchar green, uchar blue);

    virtual bool setBrightness(uchar brightness);
    virtual bool getBrightness(uchar *brightness);

    bool setSpectrumInit();

private:
    bool setMatrixEffect(RazerMatrixEffectId effect,
                         uchar arg1 = 0x00, uchar arg2 = 0x00, uchar arg3 = 0x00, uchar arg4 = 0x00, uchar arg5 = 0x00, uchar arg6 = 0x00, uchar arg7 = 0x00, uchar arg8 = 0x00);
    bool setMouseMatrixEffect(RazerMouseMatrixEffectId effect,
                              uchar arg3 = 0x00, uchar arg4 = 0x00, uchar arg5 = 0x00, uchar arg6 = 0x00, uchar arg7 = 0x00, uchar arg8 = 0x00, uchar arg9 = 0x00, uchar arg10 = 0x00, uchar arg11 = 0x00);
};

#endif // RAZERMATRIXLED_H
