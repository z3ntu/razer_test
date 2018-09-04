#ifndef MYPLUGIN
#define MYPLUGIN

#include <QObject>
#include <QDBusMetaType>

#include "plugininterface.h"

// Copied from razerdevice.h
struct RazerDPI {
    ushort dpi_x;
    ushort dpi_y;
};
Q_DECLARE_METATYPE(RazerDPI)
// Copied from razerdevice.h

class MyPlugin : public QObject, public QDBusCpp2XmlPlugin
{
    Q_OBJECT
    Q_INTERFACES(QDBusCpp2XmlPlugin)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.DBus.Cpp2XmlPlugin")

public:
    virtual void registerMetaTypes();
};

#endif //MYPLUGIN

