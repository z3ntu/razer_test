#include "myplugin.h"
#include "razerled.h"
#include <QMetaType>
#include <QVector>

void MyPlugin::registerMetaTypes()
{
    qRegisterMetaType<RazerLedId>("RazerLedId");
    qDBusRegisterMetaType<RazerLedId>();

    // Register QVector<RazerLedId> as well as otherwise getLedIds() returns nothing.
    qDBusRegisterMetaType<QVector<RazerLedId>>();

    qRegisterMetaType<WaveDirection>("WaveDirection");
    qDBusRegisterMetaType<WaveDirection>();

    qRegisterMetaType<RazerDPI>("RazerDPI");
    qDBusRegisterMetaType<RazerDPI>();

    qRegisterMetaType<ReactiveSpeed>("ReactiveSpeed");
    qDBusRegisterMetaType<ReactiveSpeed>();

    qDBusRegisterMetaType<QList<QDBusObjectPath>>();
}

// Copied from razerdevice.h
// Marshall the RazerDPI data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const RazerDPI &razerDPI)
{
    argument.beginStructure();
    argument << razerDPI.dpi_x << razerDPI.dpi_y;
    argument.endStructure();
    return argument;
}

// Retrieve the RazerDPI data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RazerDPI &razerDPI)
{
    argument.beginStructure();
    argument >> razerDPI.dpi_x >> razerDPI.dpi_y;
    argument.endStructure();
    return argument;
}
// Copied from razerdevice.h
