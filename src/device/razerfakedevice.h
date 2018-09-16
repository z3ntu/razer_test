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

#ifndef RAZERFAKEDEVICE_H
#define RAZERFAKEDEVICE_H

#include "razerdevice.h"

/**
 * @todo write docs
 */
class RazerFakeDevice : public RazerDevice
{
    using RazerDevice::RazerDevice;

    bool openDeviceHandle() override;

    bool initializeLeds() override;

    QString getSerial() override;
    QString getFirmwareVersion() override;
    QString getKeyboardLayout() override;

    virtual bool setNone(RazerLedId led);
    virtual bool setStatic(RazerLedId led, uchar red, uchar green, uchar blue);
    virtual bool setBreathing(RazerLedId led, uchar red, uchar green, uchar blue);
    virtual bool setBreathingDual(RazerLedId led, uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2);
    virtual bool setBreathingRandom(RazerLedId led);
    virtual bool setBlinking(RazerLedId led, uchar red, uchar green, uchar blue);
    virtual bool setSpectrum(RazerLedId led);
    virtual bool setWave(RazerLedId led, WaveDirection direction);
    virtual bool setReactive(RazerLedId led, ReactiveSpeed speed, uchar red, uchar green, uchar blue);

    bool displayCustomFrame() override;
    bool defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QByteArray rgbData) override;

    virtual bool setBrightness(RazerLedId led, uchar brightness);
    virtual bool getBrightness(RazerLedId led, uchar *brightness);

private:
    QString serial;
    QString fwVersion;

    static int serialCounter;
};

#endif // RAZERFAKEDEVICE_H
