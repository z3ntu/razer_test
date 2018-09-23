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

#ifndef RAZERFAKELED_H
#define RAZERFAKELED_H

#include "razerled.h"

/**
 * @todo write docs
 */
class RazerFakeLED : public RazerLED
{
public:
    using RazerLED::RazerLED;

//     RazerEffect effect;

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
};

#endif // RAZERFAKELED_H
