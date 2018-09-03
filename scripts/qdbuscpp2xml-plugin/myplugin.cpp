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

    qDBusRegisterMetaType<QList<QDBusObjectPath>>();
}
