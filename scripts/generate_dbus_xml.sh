#!/bin/bash

set -e

# See https://techbase.kde.org/Development/Tutorials/D-Bus/Cpp2XmlPlugins
# If you need the patch for Qt 5.11, message me.

env QT_PLUGIN_PATH=scripts/qdbuscpp2xml-plugin/builddir/ \
    /mnt/hdd/qt/qtbase/bin/qdbuscpp2xml -x qdbuscpp2xml-plugin \
    src/device/razerdevice.h -o razerdevice.xml

env QT_PLUGIN_PATH=scripts/qdbuscpp2xml-plugin/builddir/ \
    /mnt/hdd/qt/qtbase/bin/qdbuscpp2xml -x qdbuscpp2xml-plugin \
    src/manager/devicemanager.h -o devicemanager.xml
