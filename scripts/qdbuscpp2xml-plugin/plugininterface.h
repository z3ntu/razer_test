#ifndef SOMEMYINCLUDE_H
#define SOMEMYINCLUDE_H

#include <QtPlugin>

class QDBusCpp2XmlPlugin
{
public:
    virtual ~QDBusCpp2XmlPlugin() {}
    virtual void registerMetaTypes() = 0;
};
Q_DECLARE_INTERFACE(QDBusCpp2XmlPlugin, "org.qt-project.Qt.DBus.Cpp2XmlPlugin")
#endif // SOMEMYINCLUDE_H
