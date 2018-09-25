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

#ifndef RAZERLED_H
#define RAZERLED_H

#include <QMetaType>
#include <QDBusArgument>
#include <QDBusContext>
#include <QList>

#include "../razer_test.h"
#include "../razer_test_private.h"

using namespace razer_test;

// Forward-declare RazerDevice and resolve it at the bottom of this file.
// Resolves a circular dependency between RazerLED and RazerDevice.
class RazerDevice;

/**
 * @todo write docs
 */
class RazerLED : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "io.github.openrazer1.Led")
    Q_PROPERTY(QList<RGB> CurrentColors READ getCurrentColors)
    Q_PROPERTY(RazerEffect CurrentEffect READ getCurrentEffect)

public:
    RazerLED(RazerDevice *device, RazerLedId ledId);
    ~RazerLED() override;

    virtual bool initialize() = 0;

    virtual bool getBrightness(uchar *brightness) = 0;

    QDBusObjectPath getObjectPath();
    RazerEffect getCurrentEffect();
    QList<RGB> getCurrentColors();

    RazerDevice *device;
    const RazerLedId ledId;
    uchar brightness;

public Q_SLOTS:
    // FX
    virtual bool setNone() = 0;
    virtual bool setStatic(RGB color) = 0;
    virtual bool setBreathing(RGB color) = 0;
    virtual bool setBreathingDual(RGB color, RGB color2) = 0;
    virtual bool setBreathingRandom() = 0;
    virtual bool setBlinking(RGB color) = 0;
    virtual bool setSpectrum() = 0;
    virtual bool setWave(WaveDirection direction) = 0;
    virtual bool setReactive(ReactiveSpeed speed, RGB color) = 0;

    virtual bool setBrightness(uchar brightness) = 0;
    uchar getBrightness();

protected:
    bool checkFx(QString fxStr);
    void saveFxAndColors(RazerEffect fx, int numColors, RGB color1 = {0, 0, 0}, RGB color2 = {0, 0, 0}, RGB color3 = {0, 0, 0});

    RazerEffect effect = RazerEffect::Spectrum;
    RGB color1 = {0, 255, 0};
    RGB color2 = {255, 0, 0};
    RGB color3 = {0, 0, 255};
};

#include "../device/razerdevice.h"

#endif // RAZERLED_H
