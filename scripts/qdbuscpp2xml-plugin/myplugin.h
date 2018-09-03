#ifndef MYPLUGIN
#define MYPLUGIN

#include <QObject>
#include <QDBusMetaType>

#include "plugininterface.h"

class MyPlugin : public QObject, public QDBusCpp2XmlPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDBusCpp2XmlPlugin)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.DBus.Cpp2XmlPlugin")

public:
    virtual void registerMetaTypes();
};

#endif //MYPLUGIN

