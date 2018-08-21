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

#ifndef RAZERMATRIXDEVICE_H
#define RAZERMATRIXDEVICE_H

#include "razerdevice.h"

/**
 * @todo write docs
 */
class RazerMatrixDevice : public RazerDevice
{
    using RazerDevice::RazerDevice;

    virtual bool initializeLeds();

    virtual bool setNone(RazerLedId led);
    virtual bool setStatic(RazerLedId led, uchar red, uchar green, uchar blue);
    virtual bool setBreathing(RazerLedId led, uchar red, uchar green, uchar blue);
    virtual bool setBreathingDual(RazerLedId led, uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2);
    virtual bool setBreathingRandom(RazerLedId led);
    virtual bool setBlinking(RazerLedId led, uchar red, uchar green, uchar blue);
    virtual bool setSpectrum(RazerLedId led);
    virtual bool setWave(RazerLedId led, WaveDirection direction);
    virtual bool setCustomFrame(RazerLedId led);

    virtual bool setBrightness(RazerLedId led, uchar brightness);
    virtual bool getBrightness(RazerLedId led, uchar *brightness);
private:
    bool setSpectrumInit(RazerLedId led);
    bool setMatrixEffect(RazerLedId led, RazerMatrixEffectId effect,
                         uchar arg1 = 0x00, uchar arg2 = 0x00, uchar arg3 = 0x00, uchar arg4 = 0x00, uchar arg5 = 0x00, uchar arg6 = 0x00, uchar arg7 = 0x00, uchar arg8 = 0x00);
};

#endif // RAZERMATRIXDEVICE_H
