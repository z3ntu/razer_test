#include "myplugin.h"
#include "razer_test.h"
#include <QMetaType>
#include <QVector>

using namespace razer_test;

void MyPlugin::registerMetaTypes()
{
    qRegisterMetaType<RazerLedId>("RazerLedId");
    qDBusRegisterMetaType<RazerLedId>();

    qRegisterMetaType<WaveDirection>("WaveDirection");
    qDBusRegisterMetaType<WaveDirection>();

    qRegisterMetaType<RazerDPI>("RazerDPI");
    qDBusRegisterMetaType<RazerDPI>();

    qRegisterMetaType<ReactiveSpeed>("ReactiveSpeed");
    qDBusRegisterMetaType<ReactiveSpeed>();

    qRegisterMetaType<MatrixDimensions>("MatrixDimensions");
    qDBusRegisterMetaType<MatrixDimensions>();

    qDBusRegisterMetaType<QList<QDBusObjectPath>>();
}
