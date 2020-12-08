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

#include "daemon.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QTextStream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <hidapi.h>

#ifdef ENABLE_BRINGUP_UTIL
#include "bringup/bringuputil.h"
#endif
#include "config.h"
#include "dbus/devicemanageradaptor.h"
#include "dbus/razerdeviceadaptor.h"
#include "dbus/razerledadaptor.h"
#include "device/razerclassicdevice.h"
#include "device/razerdevice.h"
#include "device/razerfakedevice.h"
#include "device/razermatrixdevice.h"
#include "led/razerclassicled.h"
#include "led/razerled.h"

Daemon::Daemon(bool develMode, bool fakeDevices)
    : connection(RAZER_TEST_DBUS_BUS), develMode(develMode), fakeDevices(fakeDevices)
{
#ifndef NO_DEVNOTIFIER_IMPL
    if (!fakeDevices) {
        // Set up the Device Notifier
        notifier = new DeviceNotifier();
        notifier->setup();
        connect(notifier, &IDeviceNotifier::triggerRediscover, this, &Daemon::rediscoverDevices);
    } else {
        qWarning("Skipping DeviceNotifier setup because of fake-devices mode.");
    }
#endif
}

bool Daemon::initialize()
{
    // Load the supported devices from the json files
    supportedDevices = loadDevicesFromJson(develMode);
    if (supportedDevices.isEmpty()) {
        qCritical("JSON device definition files were not found. Exiting.");
        return false;
    }

    // Use the real devices
    if (!fakeDevices) {
        // Initialize HIDAPI
        if (hid_init())
            return false;

        discoverDevices();
    } else { // Handle fake devices
        discoverFakeDevices();
    }

    manager = new DeviceManager(devices);
    connect(this, &Daemon::devicesChanged, manager, &DeviceManager::setDevices);
    new DeviceManagerAdaptor(manager);
    if (!connection.registerObject(manager->getObjectPath().path(), manager)) {
        qCritical("Failed to register D-Bus object at \"%s\".", qUtf8Printable(manager->getObjectPath().path()));
        return false;
    }

#ifdef ENABLE_BRINGUP_UTIL

    if (devices.isEmpty()) {
        qCritical("No device found. Exiting.");
        return false;
    }
    foreach (RazerDevice *device, devices) {
        BringupUtil bringupUtil = BringupUtil(device);
        bringupUtil.existingDevice();
    }
    return false;

#else

    if (!connection.registerService("io.github.openrazer1")) {
        qCritical("Failed to register D-Bus service at \"io.github.openrazer1\".");
        if (connection.lastError().isValid()) {
            qCritical("Additional information:");
            qCritical("%s", qUtf8Printable(connection.lastError().name()));
            qCritical("%s", qUtf8Printable(connection.lastError().message()));
        } else {
            qCritical("Maybe it's already running?");
        }
        return false;
    }

    qInfo("Initialization finished. D-Bus interface is active...");

    return true;

#endif
}

void Daemon::discoverDevices()
{
    struct hid_device_info *devs, *cur_dev;
    devs = hid_enumerate(0x1532, 0x0000);
    cur_dev = devs;

    // List, that is used to "cross off" devices that are found,
    // the rest is removed at the end.
    QHash<QString, RazerDevice *> deviceCheckList = QHash<QString, RazerDevice *>(devicesHash);

    while (cur_dev) {
        // Usage Page 0x01: Generic Desktop Controls
        // Usage 0x80: System Control
        // See also https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
        // TODO maybe needs https://github.com/cyanogen/uchroma/blob/2b8485e5ac931980bacb125b8dff7b9a39ea527f/uchroma/server/device_manager.py#L141-L155
        if (cur_dev->usage_page != 0x1 || cur_dev->usage != 0x80) {
            qDebug() << "Ignored interface with usage_page" << cur_dev->usage_page << "& usage" << cur_dev->usage;
            cur_dev = cur_dev->next;
            continue;
        }

        QString devPath = QString(cur_dev->path);
        // Check, if the device was added already
        if (deviceCheckList.contains(devPath)) {
            deviceCheckList.remove(devPath);
            cur_dev = cur_dev->next;
            continue;
        }

        // Get the JSON object for a device
        QJsonObject deviceObj = getDeviceJsonForDevice(cur_dev->vendor_id, cur_dev->product_id);
        // Device is not supported
        if (deviceObj.size() == 0) {
#ifdef ENABLE_BRINGUP_UTIL
            BringupUtil bringupUtil = BringupUtil(cur_dev);
            if (!bringupUtil.newDevice()) {
                break;
            }
#else
            qWarning("Device with the USB ID %s:%s (%s) is not supported.",
                     qUtf8Printable(hexUshortToString(cur_dev->vendor_id)),
                     qUtf8Printable(hexUshortToString(cur_dev->product_id)),
                     qUtf8Printable(QString::fromWCharArray(cur_dev->product_string)));
#endif
            cur_dev = cur_dev->next;
            continue;
        }

        RazerDevice *device = initializeDevice(devPath, deviceObj);
        if (device == nullptr) {
            cur_dev = cur_dev->next;
            continue;
        }

        devices.append(device);
        devicesHash.insert(devPath, device);

        // D-Bus
        registerDeviceOnDBus(device);

        cur_dev = cur_dev->next;
    }

    // Free devs and cur_dev pointers
    hid_free_enumeration(devs);

    // Clear up devices that were removed
    QHashIterator<QString, RazerDevice *> i(deviceCheckList);
    while (i.hasNext()) {
        i.next();
        QString path = i.key();
        RazerDevice *device = i.value();

        qDebug() << "Removing device" << path;
        unregisterDeviceOnDBus(device);
        if (devicesHash.remove(path) != 1) {
            qCritical("Failed to remove path from devicesHash.");
        }
        if (!devices.removeOne(device)) {
            qCritical("Failed to remove device from devices list.");
        }
        delete device;
    }

    // Emit signal with new device list
    emit devicesChanged(devices);
}

void Daemon::discoverFakeDevices()
{
    // Check if device is supported
    for (const QJsonValue &deviceVal : supportedDevices) {
        RazerDevice *device = initializeDevice(nullptr, deviceVal.toObject());
        if (device == nullptr)
            continue;

        devices.append(device);

        // D-Bus
        registerDeviceOnDBus(device);
    }
}

void Daemon::rediscoverDevices()
{
    qDebug() << "Rediscovering devices because of a hotplug...";
    discoverDevices();
}

/**
 * Initializes the RazerDevice object with the data provided.
 * If dev_path is NULL, a fake device is created, otherwise the matching device based on the input JSON is created.
 * Returns NULL on error (error message is printed with qCritical), or a valid RazerDevice*.
 */
RazerDevice *Daemon::initializeDevice(QString dev_path, QJsonObject deviceObj)
{
    qInfo().noquote().nospace() << "Initializing device: " << deviceObj["name"].toString() << " (" << deviceObj["vid"].toString() << ":" << deviceObj["pid"].toString() << ")";

    ushort vid, pid;
    if (!getVidPidFromJson(deviceObj, &vid, &pid))
        return nullptr; // Message is printed in that method

    QString name, type, pclass;
    QVector<RazerLedId> leds;
    QStringList fx;
    QStringList features;
    QVector<RazerDeviceQuirks> quirks;
    MatrixDimensions matrixDimensions;
    ushort maxDPI;
    if (!getDeviceInfoFromJson(deviceObj, &name, &type, &pclass, &leds, &fx, &features, &quirks, &matrixDimensions, &maxDPI)) {
        qCritical("Failed to get device info from JSON");
        return nullptr;
    }

    RazerDevice *device;
    if (dev_path == nullptr) { // create a fake device
        device = new RazerFakeDevice(dev_path, vid, pid, name, type, leds, fx, features, quirks, matrixDimensions, maxDPI);
    } else if (pclass == "classic") {
        device = new RazerClassicDevice(dev_path, vid, pid, name, type, leds, fx, features, quirks, matrixDimensions, maxDPI);
    } else if (pclass == "matrix") {
        device = new RazerMatrixDevice(dev_path, vid, pid, name, type, leds, fx, features, quirks, matrixDimensions, maxDPI);
    } else {
        qCritical("Unknown device class: %s", qUtf8Printable(pclass));
        return nullptr;
    }
    if (!device->openDeviceHandle()) {
        qCritical("Failed to open device handle, skipping device.");
        delete device;
        return nullptr;
    }
    if (!device->initialize()) {
        qCritical("Failed to initialize leds, skipping device.");
        delete device;
        return nullptr;
    }
    return device;
}

QJsonArray Daemon::loadDevicesFromJson(bool devel)
{
    QJsonArray devices;

    QDir datadir;
    if (devel)
        datadir = QDir("../data/devices");
    else
        datadir = QDir(RAZER_TEST_DATADIR);

    QStringList filters;
    filters << "*.json";
    datadir.setNameFilters(filters);

    QListIterator<QFileInfo> i(datadir.entryInfoList());
    while (i.hasNext()) {
        QFile f(i.next().absoluteFilePath());
        f.open(QFile::ReadOnly);
        QJsonArray a = QJsonDocument::fromJson(f.readAll()).array();
        for (const QJsonValue &value : a) {
            devices.append(value);
        }
    }
    return devices;
}

bool Daemon::getVidPidFromJson(QJsonObject deviceObj, ushort *vid, ushort *pid)
{
    bool ok;
    *vid = deviceObj.value("vid").toString().toUShort(&ok, 16);
    if (!ok) {
        qCritical() << "Error converting vid: " << deviceObj.value("vid");
        return false;
    }
    *pid = deviceObj.value("pid").toString().toUShort(&ok, 16);
    if (!ok) {
        qCritical() << "Error converting pid: " << deviceObj.value("pid");
        return false;
    }
    return true;
}

bool Daemon::registerDeviceOnDBus(RazerDevice *device)
{
    // D-Bus
    new RazerDeviceAdaptor(device);
    if (!connection.registerObject(device->getObjectPath().path(), device)) {
        qCritical("Failed to register D-Bus object at \"%s\".", qUtf8Printable(device->getObjectPath().path()));
        delete device;
        return false;
    }
    foreach (RazerLED *led, device->getLeds()) {
        new RazerLEDAdaptor(led);
        if (!connection.registerObject(led->getObjectPath().path(), led)) {
            qCritical("Failed to register D-Bus object at \"%s\".", qUtf8Printable(led->getObjectPath().path()));
            delete device;
            return false;
        }
    }
    return true;
}

void Daemon::unregisterDeviceOnDBus(RazerDevice *device)
{
    connection.unregisterObject(device->getObjectPath().path(), QDBusConnection::UnregisterTree);
}

bool Daemon::getDeviceInfoFromJson(QJsonObject deviceObj, QString *name, QString *type, QString *pclass, QVector<RazerLedId> *leds, QStringList *fx, QStringList *features, QVector<RazerDeviceQuirks> *quirks, MatrixDimensions *matrixDimensions, ushort *maxDPI)
{
    // TODO: Check everything for sanity
    *name = deviceObj["name"].toString();
    *type = deviceObj["type"].toString();
    *pclass = deviceObj["pclass"].toString();
    for (const QJsonValue &ledVal : deviceObj["leds"].toArray()) {
        leds->append(static_cast<RazerLedId>(ledVal.toInt()));
    }
    for (const QJsonValue &fxVal : deviceObj["fx"].toArray()) {
        fx->append(fxVal.toString());
    }
    for (const QJsonValue &featureVal : deviceObj["features"].toArray()) {
        features->append(featureVal.toString());
    }
    for (const QJsonValue &quirkVal : deviceObj["quirks"].toArray()) {
        if (StringToQuirks.contains(quirkVal.toString())) {
            quirks->append(StringToQuirks.value(quirkVal.toString()));
        } else {
            qCritical("Unhandled quirks string!");
        }
    }
    *matrixDimensions = { deviceObj["matrix_dimensions"].toArray().at(0).toVariant().value<uchar>(),
                          deviceObj["matrix_dimensions"].toArray().at(1).toVariant().value<uchar>() };
    *maxDPI = deviceObj["max_dpi"].toInt();
    return true;
}

QJsonObject Daemon::getDeviceJsonForDevice(ushort vid, ushort pid)
{
    // Check if device is supported
    for (const QJsonValue &deviceVal : supportedDevices) {
        QJsonObject deviceObj = deviceVal.toObject();

        ushort vidJson, pidJson;
        if (!getVidPidFromJson(deviceObj, &vidJson, &pidJson))
            break;

        if (vid == vidJson && pid == pidJson) {
            return deviceObj;
        }
    }
    return QJsonObject();
}

QString Daemon::hexUshortToString(ushort number)
{
    return QString::number(number, 16).rightJustified(4, '0');
}
