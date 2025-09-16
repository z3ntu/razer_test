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

#ifndef RAZERLED_H
#define RAZERLED_H

#include "../razer_test_private.h"
#include "../razerreport.h"
#include "openrazer.h"

#include <QDBusArgument>
#include <QDBusContext>
#include <QList>
#include <QMetaType>

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
    Q_PROPERTY(QVector<openrazer::RGB> CurrentColors READ getCurrentColors)
    Q_PROPERTY(openrazer::Effect CurrentEffect READ getCurrentEffect)
    Q_PROPERTY(openrazer::LedId LedId READ getLedId)

public:
    RazerLED(RazerDevice *device, openrazer::LedId ledId);
    ~RazerLED() override;

    virtual bool initialize() = 0;

    virtual bool getBrightness(uchar *brightness) = 0;

    QDBusObjectPath getObjectPath();
    openrazer::Effect getCurrentEffect();
    QVector<openrazer::RGB> getCurrentColors();
    openrazer::LedId getLedId();

    RazerDevice *device;
    const openrazer::LedId ledId;
    uchar brightness;

public slots:
    // FX
    virtual bool setOff() = 0;
    virtual bool setOn() = 0;
    virtual bool setStatic(openrazer::RGB color) = 0;
    virtual bool setBreathing(openrazer::RGB color) = 0;
    virtual bool setBreathingDual(openrazer::RGB color, openrazer::RGB color2) = 0;
    virtual bool setBreathingRandom() = 0;
    virtual bool setBlinking(openrazer::RGB color) = 0;
    virtual bool setSpectrum() = 0;
    virtual bool setWave(openrazer::WaveDirection direction) = 0;
    virtual bool setReactive(openrazer::ReactiveSpeed speed, openrazer::RGB color) = 0;

    virtual bool setBrightness(uchar brightness) = 0;
    uchar getBrightness();

protected:
    bool checkFx(QString fxStr);
    bool hasFx(QString fxStr);
    void saveFxAndColors(openrazer::Effect fx, int numColors, openrazer::RGB color1 = { 0, 0, 0 }, openrazer::RGB color2 = { 0, 0, 0 }, openrazer::RGB color3 = { 0, 0, 0 });

    // D-Bus replies don't work when called from another object
    bool sendReportDBusHelper(razer_report request_report, razer_report *response_report);
    void dbusFailedHelper(const QString &errorMessage);
    void dbusNotSupportedHelper(const QString &errorMessage);

    openrazer::Effect effect = openrazer::Effect::Spectrum;
    openrazer::RGB color1 = { 0, 255, 0 };
    openrazer::RGB color2 = { 255, 0, 0 };
    openrazer::RGB color3 = { 0, 0, 255 };
};

#include "../device/razerdevice.h"

#endif // RAZERLED_H
