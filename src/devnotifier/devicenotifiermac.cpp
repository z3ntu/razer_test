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

#include "devicenotifiermac.h"

#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <QDebug>
#include <QThread>

DeviceNotifier::~DeviceNotifier() = default;

void DeviceNotifier::deviceConnectedCallback(void *refCon, io_iterator_t iterator)
{
    auto *notif = static_cast<DeviceNotifier *>(refCon);
    kern_return_t kr;
    io_service_t usbDevice;

    while ((usbDevice = IOIteratorNext(iterator))) {
        // Get the unique ID for the device
        uint64_t entryID;
        kr = IORegistryEntryGetRegistryEntryID(usbDevice, &entryID);
        if (kr != KERN_SUCCESS) {
            qCritical("Error calling IORegistryEntryGetRegistryEntryID. kr: %d", kr);
            return;
        }

        // Get the VID for the device
        IOCFPlugInInterface **plugInInterface;
        SInt32 score;
        kr = IOCreatePlugInInterfaceForService(usbDevice, kIOUSBDeviceUserClientTypeID, kIOCFPlugInInterfaceID, &plugInInterface, &score);
        if ((kr != kIOReturnSuccess) || !plugInInterface) {
            qCritical("Error calling IOCreatePlugInInterfaceForService. kr: %d", kr);
            return;
        }

        IOUSBDeviceInterface **deviceInterface;
        HRESULT res;
        res = (*plugInInterface)->QueryInterface(plugInInterface, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID), (LPVOID *)&deviceInterface);
        (*plugInInterface)->Release(plugInInterface);

        if (res || deviceInterface == NULL) {
            qCritical("Error calling QueryInterface. res: %d", res);
            return;
        }

        UInt16 vid;
        kr = (*deviceInterface)->GetDeviceVendor(deviceInterface, &vid);
        assert(kr == kIOReturnSuccess);

        // DEC 5426 = HEX 1532
        // Check if it's a Razer device, if so, insert the data into the hash
        // And if we're supposed to emit the signal, emit it
        if (vid == 5426) {
            notif->activeDevices.insert(entryID, vid);
            if (notif->active) {
                // HACK: Sleep for a while, otherwise the device won't be found with hid_enumerate
                QThread::sleep(2);
                emit notif->triggerRediscover();
            }
        }

        kr = IOObjectRelease(usbDevice);
    }
}

void DeviceNotifier::deviceDisconnectedCallback(void *refCon, io_iterator_t iterator)
{
    auto *notif = static_cast<DeviceNotifier *>(refCon);
    kern_return_t kr;
    io_service_t usbDevice;

    while ((usbDevice = IOIteratorNext(iterator))) {
        // Get the unique ID for the device
        uint64_t entryID;
        kr = IORegistryEntryGetRegistryEntryID(usbDevice, &entryID);
        if (kr != KERN_SUCCESS) {
            qCritical("Error calling IORegistryEntryGetRegistryEntryID. kr: %d", kr);
            return;
        }

        // Look up VID for the ID
        UInt16 vid = notif->activeDevices.value(entryID);
        // Check if it's a Razer device, if so, emit the signal
        if (vid == 5426) {
            emit notif->triggerRediscover();
        }
        // Remove value from hash as we've used it already
        notif->activeDevices.remove(entryID);

        kr = IOObjectRelease(usbDevice);
    }
}

bool DeviceNotifier::setup()
{
    CFMutableDictionaryRef matchingDict = IOServiceMatching(kIOUSBDeviceClassName);

    IONotificationPortRef notificationPort = IONotificationPortCreate(kIOMasterPortDefault);
    CFRunLoopSourceRef runLoopSource = IONotificationPortGetRunLoopSource(notificationPort);

    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopDefaultMode);

    // IOServiceAddMatchingNotification releases this, so we retain for the next call
    CFRetain(matchingDict);

    kern_return_t kr;
    kr = IOServiceAddMatchingNotification(notificationPort,
                                          kIOMatchedNotification,
                                          matchingDict,
                                          deviceConnectedCallback,
                                          this,
                                          &gAddedIter);

    kr = IOServiceAddMatchingNotification(notificationPort,
                                          kIOTerminatedNotification,
                                          matchingDict,
                                          deviceDisconnectedCallback,
                                          this,
                                          &gRemovedIter);

    // Iterate once to get already-present devices and arm the notification
    deviceConnectedCallback(this, gAddedIter);
    deviceDisconnectedCallback(this, gRemovedIter);

    // Tell the functions to emit the signal.
    // This is neccessary because we have to call the callbacks and we don't want the signals to be emitted yet.
    active = true;

    return true;
}
