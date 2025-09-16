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

    bool initialize() override;

    bool setOff() override;
    bool setOn() override;
    bool setStatic(openrazer::RGB color) override;
    bool setBreathing(openrazer::RGB color) override;
    bool setBreathingDual(openrazer::RGB color, openrazer::RGB color2) override;
    bool setBreathingRandom() override;
    bool setBlinking(openrazer::RGB color) override;
    bool setSpectrum() override;
    bool setWave(openrazer::WaveDirection direction) override;
    bool setReactive(openrazer::ReactiveSpeed speed, openrazer::RGB color) override;

    bool setBrightness(uchar brightness) override;
    bool getBrightness(uchar *brightness) override;
};

#endif // RAZERFAKELED_H
