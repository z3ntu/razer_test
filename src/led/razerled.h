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
    // TODO Q_PROPERTY something?

public:
    RazerLED(RazerDevice *device, RazerLedId ledId);
    virtual ~RazerLED();

    RazerDevice *device;
    RazerLedId ledId;
    uchar brightness;

    QDBusObjectPath getObjectPath();

    virtual bool getBrightness(uchar *brightness) = 0;

public Q_SLOTS:
    // FX
    virtual bool setNone() = 0;
    virtual bool setStatic(uchar red, uchar green, uchar blue) = 0;
    virtual bool setBreathing(uchar red, uchar green, uchar blue) = 0;
    virtual bool setBreathingDual(uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2) = 0;
    virtual bool setBreathingRandom() = 0;
    virtual bool setBlinking(uchar red, uchar green, uchar blue) = 0;
    virtual bool setSpectrum() = 0;
    virtual bool setWave(WaveDirection direction) = 0;
    virtual bool setReactive(ReactiveSpeed speed, uchar red, uchar green, uchar blue) = 0;

    virtual bool setBrightness(uchar brightness) = 0;
    uchar getBrightness();
};

#include "../device/razerdevice.h"

#endif // RAZERLED_H
