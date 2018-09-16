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

/**
 * @todo write docs
 */
class RazerMatrixLED : public RazerLED
{
public:
    using RazerLED::RazerLED;

    RazerMatrixEffectId effect;
    RazerMouseMatrixEffectId mouseMatrixEffect;

    bool setNone() override;
    bool setStatic(uchar red, uchar green, uchar blue) override;
    bool setBreathing(uchar red, uchar green, uchar blue) override;
    bool setBreathingDual(uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2) override;
    bool setBreathingRandom() override;
    bool setBlinking(uchar red, uchar green, uchar blue) override;
    bool setSpectrum() override;
    bool setWave(WaveDirection direction) override;
    bool setReactive(ReactiveSpeed speed, uchar red, uchar green, uchar blue) override;

    bool setBrightness(uchar brightness) override;
    bool getBrightness(uchar *brightness) override;

    bool setSpectrumInit();

    bool setMatrixEffect(RazerMatrixEffectId effect,
                         uchar arg1 = 0x00, uchar arg2 = 0x00, uchar arg3 = 0x00, uchar arg4 = 0x00, uchar arg5 = 0x00, uchar arg6 = 0x00, uchar arg7 = 0x00, uchar arg8 = 0x00);
    bool setMouseMatrixEffect(RazerMouseMatrixEffectId effect,
                              uchar arg3 = 0x00, uchar arg4 = 0x00, uchar arg5 = 0x00, uchar arg6 = 0x00, uchar arg7 = 0x00, uchar arg8 = 0x00, uchar arg9 = 0x00, uchar arg10 = 0x00, uchar arg11 = 0x00);
};

#endif // RAZERMATRIXLED_H
