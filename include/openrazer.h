// Copyright (C) 2018-2019  Luca Weiss <luca (at) z3ntu (dot) xyz>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPENRAZER_H
#define OPENRAZER_H

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

namespace openrazer {
Q_NAMESPACE

enum class LedId : uchar {
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
    LeftSideLED = 0x11,
    ChargingLED = 0x20,
    FastChargingLED = 0x21,
    FullyChargedLED = 0x22,
};
Q_ENUM_NS(LedId)

// Marshall the LedId data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const LedId &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the LedId data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, LedId &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<LedId>(a);
    return argument;
}
inline uint qHash(LedId key, uint seed)
{
    return ::qHash(static_cast<uchar>(key), seed);
}

enum class WaveDirection : uchar {
    LEFT_TO_RIGHT = 0x01,
    RIGHT_TO_LEFT = 0x02
};
Q_ENUM_NS(WaveDirection)

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

enum class WheelDirection : uchar {
    CLOCKWISE = 0x01,
    COUNTER_CLOCKWISE = 0x02
};
Q_ENUM_NS(WheelDirection)

// Marshall the WheelDirection data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const WheelDirection &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the WheelDirection data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, WheelDirection &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<WheelDirection>(a);
    return argument;
}

enum class ReactiveSpeed : uchar {
    _500MS = 0x01,
    _1000MS = 0x02,
    _1500MS = 0x03,
    _2000MS = 0x04
};
Q_ENUM_NS(ReactiveSpeed)

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

enum class Effect {
    Off,
    On,
    Static,
    Breathing,
    BreathingDual,
    BreathingRandom,
    BreathingMono,
    Blinking,
    Spectrum,
    Wave,
    Wheel,
    Reactive,
    Ripple,
    RippleRandom,
};
Q_ENUM_NS(Effect)

// Marshall the Effect data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const Effect &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the Effect data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, Effect &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<Effect>(a);
    return argument;
}
inline uint qHash(Effect key, uint seed)
{
    return ::qHash(static_cast<uint>(key), seed);
}

struct DPI {
    ushort dpi_x;
    ushort dpi_y;
};

// Marshall the DPI data into a D-Bus argument
inline QDBusArgument &operator<<(QDBusArgument &argument, const DPI &value)
{
    argument.beginStructure();
    argument << value.dpi_x << value.dpi_y;
    argument.endStructure();
    return argument;
}

// Retrieve the DPI data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, DPI &value)
{
    argument.beginStructure();
    argument >> value.dpi_x >> value.dpi_y;
    argument.endStructure();
    return argument;
}

inline QDebug operator<<(QDebug dbg, const DPI &value)
{
    dbg.nospace() << "DPI(" << value.dpi_x << ", " << value.dpi_y << ")";
    return dbg.maybeSpace();
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

inline QDebug operator<<(QDebug dbg, const MatrixDimensions &value)
{
    dbg.nospace() << "MatrixDimensions(" << value.x << ", " << value.y << ")";
    return dbg.maybeSpace();
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

Q_DECLARE_METATYPE(openrazer::LedId)
Q_DECLARE_METATYPE(openrazer::WaveDirection)
Q_DECLARE_METATYPE(openrazer::WheelDirection)
Q_DECLARE_METATYPE(openrazer::ReactiveSpeed)
Q_DECLARE_METATYPE(openrazer::Effect)
Q_DECLARE_METATYPE(openrazer::DPI)
Q_DECLARE_METATYPE(openrazer::MatrixDimensions)
Q_DECLARE_METATYPE(openrazer::RGB)

namespace openrazer {

inline void registerMetaTypes()
{
    qRegisterMetaType<LedId>("LedId");
    qDBusRegisterMetaType<LedId>();

    qRegisterMetaType<WaveDirection>("WaveDirection");
    qDBusRegisterMetaType<WaveDirection>();

    qRegisterMetaType<WheelDirection>("WheelDirection");
    qDBusRegisterMetaType<WheelDirection>();

    qRegisterMetaType<DPI>("DPI");
    qDBusRegisterMetaType<DPI>();
    qRegisterMetaType<QVector<DPI>>("QVector<DPI>");
    qDBusRegisterMetaType<QVector<DPI>>();
    qRegisterMetaType<QPair<uchar, QVector<::openrazer::DPI>>>("QPair<uchar, QVector<DPI>>");
    qDBusRegisterMetaType<QPair<uchar, QVector<::openrazer::DPI>>>();

    qRegisterMetaType<ReactiveSpeed>("ReactiveSpeed");
    qDBusRegisterMetaType<ReactiveSpeed>();

    qRegisterMetaType<MatrixDimensions>("MatrixDimensions");
    qDBusRegisterMetaType<MatrixDimensions>();

    qRegisterMetaType<RGB>("RGB");
    qDBusRegisterMetaType<RGB>();
    qRegisterMetaType<QVector<RGB>>("QVector<RGB>");
    qDBusRegisterMetaType<QVector<RGB>>();

    qRegisterMetaType<Effect>("Effect");
    qDBusRegisterMetaType<Effect>();

    qDBusRegisterMetaType<QList<QDBusObjectPath>>();
}

}

#endif // OPENRAZER_H
