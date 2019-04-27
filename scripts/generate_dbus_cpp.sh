#!/bin/sh

set -e

# Move only afterwards that header file path is correct
qdbusxml2cpp dbus/razerdevice.xml -a razerdeviceadaptor.h:razerdeviceadaptor.cpp -c RazerDeviceAdaptor
mv razerdeviceadaptor.h razerdeviceadaptor.cpp src/dbus/
# add razer_test.h include
sed -i '/#define RAZERDEVICEADAPTOR_H/a #include "razer_test.h"\nusing namespace razer_test;' src/dbus/razerdeviceadaptor.h


qdbusxml2cpp dbus/devicemanager.xml -a devicemanageradaptor.h:devicemanageradaptor.cpp -c DeviceManagerAdaptor
mv devicemanageradaptor.h devicemanageradaptor.cpp src/dbus/


qdbusxml2cpp dbus/razerled.xml -a razerledadaptor.h:razerledadaptor.cpp -c RazerLEDAdaptor
mv razerledadaptor.h razerledadaptor.cpp src/dbus/
# add razer_test.h include
sed -i '/#define RAZERLEDADAPTOR_H/a #include "razer_test.h"\nusing namespace razer_test;' src/dbus/razerledadaptor.h
