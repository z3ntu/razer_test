#include "myplugin.h"

#include "razer_test.h"

#include <QMetaType>
#include <QVector>

using namespace razer_test;

void MyPlugin::registerMetaTypes()
{
    razer_test::registerMetaTypes();
}
