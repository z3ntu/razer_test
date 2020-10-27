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

#ifndef RAZERTEST_H
#define RAZERTEST_H

#include <QDBusArgument>
#include <QDBusMetaType>
#include <QDebug>

#if defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD)
#define RAZER_TEST_DBUS_BUS QDBusConnection::systemBus()
#elif defined(Q_OS_DARWIN) || defined(Q_OS_WIN)
#define RAZER_TEST_DBUS_BUS QDBusConnection::sessionBus()
#else
#error "Please choose a RAZER_TEST_DBUS_BUS for this platform!"
#endif

namespace razer_test {

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

// Marshall the RazerLedId data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const RazerLedId &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the RazerLedId data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, RazerLedId &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<RazerLedId>(a);
    return argument;
}
inline uint qHash(RazerLedId key, uint seed)
{
    return ::qHash(static_cast<uchar>(key), seed);
}

enum class WaveDirection : uchar {
    LEFT_TO_RIGHT = 0x01,
    RIGHT_TO_LEFT = 0x02
};

// Marshall the WaveDirection data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const WaveDirection &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the WaveDirection data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, WaveDirection &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<WaveDirection>(a);
    return argument;
}

enum class ReactiveSpeed : uchar {
    _500MS = 0x01,
    _1000MS = 0x02,
    _1500MS = 0x03,
    _2000MS = 0x04
};

// Marshall the ReactiveSpeed data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const ReactiveSpeed &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the ReactiveSpeed data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, ReactiveSpeed &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<ReactiveSpeed>(a);
    return argument;
}

enum class RazerEffect {
    Off,
    On,
    Static,
    Breathing,
    BreathingDual,
    BreathingRandom,
    Blinking,
    Spectrum,
    Wave,
    Reactive,
};

// Marshall the RazerEffect data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const RazerEffect &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the RazerEffect data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, RazerEffect &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<RazerEffect>(a);
    return argument;
}
inline uint qHash(RazerEffect key, uint seed)
{
    return ::qHash(static_cast<uint>(key), seed);
}

struct RazerDPI {
    ushort dpi_x;
    ushort dpi_y;
};

// Marshall the RazerDPI data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const RazerDPI &value)
{
    argument.beginStructure();
    argument << value.dpi_x << value.dpi_y;
    argument.endStructure();
    return argument;
}

// Retrieve the RazerDPI data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, RazerDPI &value)
{
    argument.beginStructure();
    argument >> value.dpi_x >> value.dpi_y;
    argument.endStructure();
    return argument;
}

struct MatrixDimensions {
    uchar x;
    uchar y;
};

// Marshall the MatrixDimensions data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const MatrixDimensions &value)
{
    argument.beginStructure();
    argument << value.x << value.y;
    argument.endStructure();
    return argument;
}

// Retrieve the MatrixDimensions data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, MatrixDimensions &value)
{
    argument.beginStructure();
    argument >> value.x >> value.y;
    argument.endStructure();
    return argument;
}

struct RGB {
    uchar r;
    uchar g;
    uchar b;
};

// Marshall the RGB data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const RGB &value)
{
    argument.beginStructure();
    argument << value.r << value.g << value.b;
    argument.endStructure();
    return argument;
}

// Retrieve the RGB data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, RGB &value)
{
    argument.beginStructure();
    argument >> value.r >> value.g >> value.b;
    argument.endStructure();
    return argument;
}

inline QDebug operator<<(QDebug dbg, const RGB &value)
{
    dbg.nospace() << "RGB(" << value.r << ", " << value.g << ", " << value.b << ")";
    return dbg.maybeSpace();
}

}

Q_DECLARE_METATYPE(razer_test::RazerLedId)
Q_DECLARE_METATYPE(razer_test::WaveDirection)
Q_DECLARE_METATYPE(razer_test::ReactiveSpeed)
Q_DECLARE_METATYPE(razer_test::RazerEffect)
Q_DECLARE_METATYPE(razer_test::RazerDPI)
Q_DECLARE_METATYPE(razer_test::MatrixDimensions)
Q_DECLARE_METATYPE(razer_test::RGB)

namespace razer_test {

inline void registerMetaTypes()
{
    qRegisterMetaType<RazerLedId>("RazerLedId");
    qDBusRegisterMetaType<RazerLedId>();

    qRegisterMetaType<WaveDirection>("WaveDirection");
    qDBusRegisterMetaType<WaveDirection>();

    qRegisterMetaType<RazerDPI>("RazerDPI");
    qDBusRegisterMetaType<RazerDPI>();
    qRegisterMetaType<QVector<RazerDPI>>("QVector<RazerDPI>");
    qDBusRegisterMetaType<QVector<RazerDPI>>();

    qRegisterMetaType<ReactiveSpeed>("ReactiveSpeed");
    qDBusRegisterMetaType<ReactiveSpeed>();

    qRegisterMetaType<MatrixDimensions>("MatrixDimensions");
    qDBusRegisterMetaType<MatrixDimensions>();

    qRegisterMetaType<RGB>("RGB");
    qDBusRegisterMetaType<RGB>();
    qRegisterMetaType<QVector<RGB>>("QVector<RGB>");
    qDBusRegisterMetaType<QVector<RGB>>();

    qRegisterMetaType<RazerEffect>("RazerEffect");
    qDBusRegisterMetaType<RazerEffect>();

    qDBusRegisterMetaType<QList<QDBusObjectPath>>();
}

}

#endif // RAZERTEST_H
