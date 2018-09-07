#!/bin/bash

set -e

# Move only afterwards that header file path is correct
qdbusxml2cpp razerdevice.xml -a razerdeviceadaptor.h:razerdeviceadaptor.cpp -c RazerDeviceAdaptor
mv razerdeviceadaptor.h razerdeviceadaptor.cpp src/dbus/
# add razer_test.h include
sed -i '/#define RAZERDEVICEADAPTOR_H/a #include "../razer_test.h"' src/dbus/razerdeviceadaptor.h


qdbusxml2cpp devicemanager.xml -a devicemanageradaptor.h:devicemanageradaptor.cpp -c DeviceManagerAdaptor
mv devicemanageradaptor.h devicemanageradaptor.cpp src/dbus/


qdbusxml2cpp razerled.xml -a razerledadaptor.h:razerledadaptor.cpp -c RazerLEDAdaptor
mv razerledadaptor.h razerledadaptor.cpp src/dbus/
# add razer_test.h include
sed -i '/#define RAZERLEDADAPTOR_H/a #include "../razer_test.h"' src/dbus/razerledadaptor.h
