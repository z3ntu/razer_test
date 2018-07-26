#!/bin/bash

qdbusxml2cpp razerdevice.xml -a src/dbus/razerdeviceadaptor.h:src/dbus/razerdeviceadaptor.cpp

# add razerled.h include
sed -i '/#define RAZERDEVICEADAPTOR_H/a #include "../led/razerled.h"' src/dbus/razerdeviceadaptor.h
