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

#include "../device/razerdevice.h"

enum class RazerLedId : uchar {
    Unspecified = 0x00,
    ScrollWheelLED = 0x01,
    BatteryLED = 0x03,
    LogoLED = 0x04,
    BacklightLED = 0x05,
    MacroRecordingLED = 0x07,
    GameModeLED = 0x08,
    KeymapRedLED = 0x0C,
    KeymapGreenLED = 0x0D,
    KeymapBlueLED = 0x0E,
    RightSideLED = 0x10,
    LeftSideLED = 0x11
};
Q_DECLARE_METATYPE(RazerLedId)

QDBusArgument &operator<<(QDBusArgument &argument, const RazerLedId &value);
const QDBusArgument &operator>>(const QDBusArgument &argument, RazerLedId &value);
inline uint qHash(RazerLedId key, uint seed)
{
    return ::qHash(static_cast<uchar>(key), seed);
}

enum class WaveDirection : uchar {
    LEFT_TO_RIGHT = 0x01,
    RIGHT_TO_LEFT = 0x02
};
Q_DECLARE_METATYPE(WaveDirection)

QDBusArgument &operator<<(QDBusArgument &argument, const WaveDirection &value);
const QDBusArgument &operator>>(const QDBusArgument &argument, WaveDirection &value);

enum class ReactiveSpeed : uchar {
    _500MS = 0x01,
    _1000MS = 0x02,
    _1500MS = 0x03,
    _2000MS = 0x04
};
Q_DECLARE_METATYPE(ReactiveSpeed)

QDBusArgument &operator<<(QDBusArgument &argument, const ReactiveSpeed &value);
const QDBusArgument &operator>>(const QDBusArgument &argument, ReactiveSpeed &value);

/**
 * @todo write docs
 */
class RazerLED : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "io.github.openrazer1.Device.Led")
    // TODO Q_PROPERTY something?

public:
    RazerLED(RazerDevice *device, RazerLedId ledId);
    virtual ~RazerLED();

    RazerDevice *device;
    RazerLedId ledId;
    uchar brightness;

    virtual bool getBrightness(RazerLedId led, uchar *brightness) = 0;

public Q_SLOTS:
    // FX
    virtual bool setNone(RazerLedId led) = 0;
    virtual bool setStatic(RazerLedId led, uchar red, uchar green, uchar blue) = 0;
    virtual bool setBreathing(RazerLedId led, uchar red, uchar green, uchar blue) = 0;
    virtual bool setBreathingDual(RazerLedId led, uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2) = 0;
    virtual bool setBreathingRandom(RazerLedId led) = 0;
    virtual bool setBlinking(RazerLedId led, uchar red, uchar green, uchar blue) = 0;
    virtual bool setSpectrum(RazerLedId led) = 0;
    virtual bool setWave(RazerLedId led, WaveDirection direction) = 0;
    virtual bool setReactive(RazerLedId led, ReactiveSpeed speed, uchar red, uchar green, uchar blue) = 0;

    virtual bool setBrightness(RazerLedId led, uchar brightness) = 0;
    uchar getBrightness(RazerLedId led);
};

#endif // RAZERLED_H
