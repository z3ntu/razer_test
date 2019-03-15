/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018-2019  Luca Weiss <luca@z3ntu.xyz>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Thanks to Benjamin Tissoires for implementing something similar in mtdiag-qt!

#include "devicenotifierlinux.h"

#include <QDebug>
#include <QSocketNotifier>

DeviceNotifier::~DeviceNotifier()
{
    udev_unref(udev);
    udev_monitor_unref(mon);
}

bool DeviceNotifier::setup()
{
    udev = udev_new();
    if (!udev) {
        qCritical("Could not call udev_new()");
        return false;
    }

    enumerateExistingDevices();

    mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", "usb_device");
    udev_monitor_enable_receiving(mon);

    auto *sn = new QSocketNotifier(udev_monitor_get_fd(mon), QSocketNotifier::Read, this);
    connect(sn, &QSocketNotifier::activated, this, &DeviceNotifier::udevEvent);
    return true;
}

void DeviceNotifier::udevEvent(int fd)
{
    struct udev_device *dev;

    if (fd != udev_monitor_get_fd(mon))
        return;

    dev = udev_monitor_receive_device(mon);

    QString action = QString(udev_device_get_action(dev));
    if (action != "add" && action != "remove") {
        udev_device_unref(dev);
        return;
    }

    QString path = QString(udev_device_get_devpath(dev));
    QString vid = QString(udev_device_get_sysattr_value(dev, "idVendor"));

    // Insert to the list, when the device is added and the VID is 1532
    // Look up, when the VID is unknown (= empty)
    if (action == "add" && vid == "1532") {
        activeDevices.insert(path, vid);
    } else if (vid == "") {
        vid = activeDevices.value(path);
    }

    if (vid == "1532") {
        emit triggerRediscover();
    }

    udev_device_unref(dev);
}

// Enumerates already-connected Razer devices and adds them to the activeDevices hash to be used in udevEvent()
void DeviceNotifier::enumerateExistingDevices()
{
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    struct udev_device *dev;

    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "usb");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    udev_list_entry_foreach(dev_list_entry, devices)
    {
        dev = udev_device_new_from_syspath(udev, udev_list_entry_get_name(dev_list_entry));

        QString devtype = QString(udev_device_get_devtype(dev));
        if (devtype != "usb_device") {
            udev_device_unref(dev);
            continue;
        }

        QString path = QString(udev_device_get_devpath(dev));
        QString vid = QString(udev_device_get_sysattr_value(dev, "idVendor"));

        // Insert to the list when the VID is 1532
        if (vid == "1532") {
            activeDevices.insert(path, vid);
        }

        udev_device_unref(dev);
    }

    udev_enumerate_unref(enumerate);
}
