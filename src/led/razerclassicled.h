/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018-2019  Luca Weiss <luca@z3ntu.xyz>
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

    bool initialize() override;

    bool setOff() override;
    bool setOn() override;
    bool setStatic(RGB color) override;
    bool setBreathing(RGB color) override;
    bool setBreathingDual(RGB color, RGB color2) override;
    bool setBreathingRandom() override;
    bool setBlinking(RGB color) override;
    bool setSpectrum() override;
    bool setWave(WaveDirection direction) override;
    bool setReactive(ReactiveSpeed speed, RGB color) override;

    bool setBrightness(uchar brightness) override;
    bool getBrightness(uchar *brightness) override;

private:
    bool setLedState(RazerClassicLedState state);
    bool getLedState(RazerClassicLedState *state);
    bool ensureLedStateOn();
    bool setLedEffect(RazerClassicEffectId effect);
    bool getLedEffect(RazerClassicEffectId *effect);
    bool setLedRgb(RGB color);
    bool getLedRgb(RGB *color);

    RazerClassicLedState classicState;

    QHash<RazerClassicEffectId, RazerEffect> effectTranslationTable {
        { RazerClassicEffectId::Static, RazerEffect::Static },
        { RazerClassicEffectId::Blinking, RazerEffect::Blinking },
        { RazerClassicEffectId::Breathing, RazerEffect::Breathing },
        { RazerClassicEffectId::Spectrum, RazerEffect::Spectrum }
    };
};

#endif // RAZERCLASSICLED_H
