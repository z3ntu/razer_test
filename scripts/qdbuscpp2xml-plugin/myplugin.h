#ifndef MYPLUGIN
#define MYPLUGIN

#include "plugininterface.h"

#include <QDBusMetaType>
#include <QObject>

class MyPlugin : public QObject, public QDBusCpp2XmlPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDBusCpp2XmlPlugin)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.DBus.Cpp2XmlPlugin")

public:
    void registerMetaTypes() override;
};

#endif //MYPLUGIN
