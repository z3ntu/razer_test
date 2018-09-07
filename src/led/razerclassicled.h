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

/**
 * @todo write docs
 */
class RazerClassicLED : public RazerLED
{
public:
    using RazerLED::RazerLED;

    RazerClassicLedState state;
    RazerClassicEffectId effect;
    unsigned char red;
    unsigned char green;
    unsigned char blue;

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

    bool setLedState(RazerClassicLedState state);
    bool getLedState(RazerClassicLedState *state);
    bool ensureLedStateOn();
    bool setLedEffect(RazerClassicEffectId effect);
    bool getLedEffect(RazerClassicEffectId *effect);
    bool setLedRgb(uchar red, uchar green, uchar blue);
    bool getLedRgb(uchar *red, uchar *green, uchar *blue);
};

#endif // RAZERCLASSICLED_H
