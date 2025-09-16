#!/bin/sh

set -e

# See https://techbase.kde.org/Development/Tutorials/D-Bus/Cpp2XmlPlugins
# If you need the patch for Qt 6.9, message me.

QDBUSCPP2XML=/mnt/hdd/qt/qtbase/build/bin/qdbuscpp2xml

env QT_PLUGIN_PATH=scripts/qdbuscpp2xml-plugin/builddir/ \
    $QDBUSCPP2XML -x qdbuscpp2xml-plugin \
    src/device/razerdevice.h -o dbus/razerdevice.xml

env QT_PLUGIN_PATH=scripts/qdbuscpp2xml-plugin/builddir/ \
    $QDBUSCPP2XML -x qdbuscpp2xml-plugin \
    src/manager/devicemanager.h -o dbus/devicemanager.xml

env QT_PLUGIN_PATH=scripts/qdbuscpp2xml-plugin/builddir/ \
    $QDBUSCPP2XML -x qdbuscpp2xml-plugin \
    src/led/razerled.h -o dbus/razerled.xml
