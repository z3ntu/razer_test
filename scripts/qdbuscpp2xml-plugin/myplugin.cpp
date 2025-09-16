#include "myplugin.h"

#include "openrazer.h"

#include <QMetaType>
#include <QVector>

void MyPlugin::registerMetaTypes()
{
    openrazer::registerMetaTypes();
}
