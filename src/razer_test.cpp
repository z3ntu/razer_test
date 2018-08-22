#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hidapi.h>

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QCoreApplication>

#include <QDBusConnection>

#include "device/razerdevice.h"
#include "device/razerclassicdevice.h"
#include "device/razermatrixdevice.h"
#include "device/razerfakedevice.h"
#include "led/razerled.h"
#include "led/razerclassicled.h"
#include "dbus/razerdeviceadaptor.h"
#include "dbus/devicemanageradaptor.h"
#include "manager/devicemanager.h"
#include "config.h"


QJsonArray loadDevicesFromJson()
{
    QJsonArray devices;

    QDir datadir("../data");
    QStringList filters;
    filters << "*.json";
    datadir.setNameFilters(filters);

    QListIterator<QFileInfo> i(datadir.entryInfoList());
    while(i.hasNext()) {
        QFile f(i.next().absoluteFilePath());
        f.open(QFile::ReadOnly);
        QJsonArray a = QJsonDocument::fromJson(f.readAll()).array();
        foreach(const QJsonValue &value, a) {
            devices.append(value);
        }
    }
    return devices;
}

RazerDevice* initializeDevice(QString dev_path, ushort vid, ushort pid, QString name, QString type, QString pclass, QVector<RazerLedId> leds, QVector<RazerDeviceQuirks> quirks)
{
    RazerDevice *device;
    if(dev_path == NULL) { // create a fake device
        device = new RazerFakeDevice(dev_path, vid, pid, name, type, pclass, leds, quirks);
    } else if(pclass == "classic") {
        device = new RazerClassicDevice(dev_path, vid, pid, name, type, pclass, leds, quirks);
    } else if(pclass == "matrix") {
        device = new RazerMatrixDevice(dev_path, vid, pid, name, type, pclass, leds, quirks);
    } else {
        qCritical("Unknown device class: %s", qUtf8Printable(pclass));
        return NULL;
    }
    if(!device->openDeviceHandle()) {
        qCritical("Failed to open device handle");
        delete device;
        return NULL;
    }
    if(!device->initializeLeds()) {
        qCritical("Failed to initialize leds");
        delete device;
        return NULL;
    }
    return device;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setApplicationName("razer_test");
    QCoreApplication::setApplicationVersion(RAZER_TEST_VERSION);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption({"fake-devices", "Adds fake devices instead of real ones."});

    parser.process(app);

    qInfo("razer_test - version %s", RAZER_TEST_VERSION);

    qRegisterMetaType<RazerLedId>("RazerLedId");
    qDBusRegisterMetaType<RazerLedId>();

    qRegisterMetaType<WaveDirection>("WaveDirection");
    qDBusRegisterMetaType<WaveDirection>();

    QDBusConnection connection = QDBusConnection::sessionBus();

    printf("sizeof(razer_report): %lu\n", sizeof(razer_report)); // should be 90

    QJsonArray supportedDevices = loadDevicesFromJson();

    QVector<RazerDevice*> devices;

    if(!parser.isSet("fake-devices")) {
        if (hid_init())
            return -1;

        struct hid_device_info *devs, *cur_dev;
        devs = hid_enumerate(0x1532, 0x0000);
        cur_dev = devs;

        QVector<ushort> devicesPid;

        while (cur_dev) {
            // TODO maybe needs https://github.com/cyanogen/uchroma/blob/2b8485e5ac931980bacb125b8dff7b9a39ea527f/uchroma/server/device_manager.py#L141-L155

            // Check if device is already added
            if(devicesPid.contains(cur_dev->product_id)) {
                cur_dev = cur_dev->next;
                continue;
            }

            // Check if device is supported
            foreach(const QJsonValue &deviceVal, supportedDevices) {
                // TODO: Improve somehow
                QJsonObject deviceObj = deviceVal.toObject();
                bool ok;
                ushort vid = deviceObj.value("vid").toString().toUShort(&ok, 16);
                if(!ok) {
                    qWarning() << "Error converting vid: " << deviceObj.value("vid");
                    continue;
                }
                ushort pid = deviceObj.value("pid").toString().toUShort(&ok, 16);
                if(!ok) {
                    qWarning() << "Error converting pid: " << deviceObj.value("pid");
                    continue;
                }

                if(cur_dev->vendor_id == vid && cur_dev->product_id == pid) {
                    qInfo().noquote().nospace() << "Initializing device: " << deviceObj.value("name").toString() << " (" << deviceObj.value("vid").toString() << ":" << deviceObj.value("pid").toString() << ")";
                    QVector<RazerLedId> leds;
                    foreach(const QJsonValue &ledVal, deviceObj["leds"].toArray()) {
                        leds.append(static_cast<RazerLedId>(ledVal.toInt()));
                    }
                    QVector<RazerDeviceQuirks> quirks;
                    foreach(const QJsonValue &quirkVal, deviceObj["quirks"].toArray()) {
                        quirks.append(static_cast<RazerDeviceQuirks>(quirkVal.toInt()));
                    }

                    RazerDevice *device = initializeDevice(QString(cur_dev->path), vid, pid, deviceObj["name"].toString(), deviceObj["type"].toString(), deviceObj["pclass"].toString(), leds, quirks);
                    if(device == NULL)
                        break;

                    devices.append(device);
                    devicesPid.append(cur_dev->product_id);

                    // D-Bus
                    new RazerDeviceAdaptor(device);
                    if(!connection.registerObject(device->getObjectPath().path(), device)) {
                        qCritical("Failed to register D-Bus object at \"%s\".", qUtf8Printable(device->getObjectPath().path()));
                        delete device;
                        break;
                    }

                    break;
                }
            }
            cur_dev = cur_dev->next;
        }

        // Free devs and cur_dev pointers
        hid_free_enumeration(devs);
    } else {
        // Check if device is supported
        foreach(const QJsonValue &deviceVal, supportedDevices) {
            // TODO: Improve somehow
            QJsonObject deviceObj = deviceVal.toObject();
            bool ok;
            ushort vid = deviceObj.value("vid").toString().toUShort(&ok, 16);
            if(!ok) {
                qWarning() << "Error converting vid: " << deviceObj.value("vid");
                continue;
            }
            ushort pid = deviceObj.value("pid").toString().toUShort(&ok, 16);
            if(!ok) {
                qWarning() << "Error converting pid: " << deviceObj.value("pid");
                continue;
            }

            qInfo().noquote().nospace() << "Initializing device: " << deviceObj.value("name").toString() << " (" << deviceObj.value("vid").toString() << ":" << deviceObj.value("pid").toString() << ")";
            QVector<RazerLedId> leds;
            foreach(const QJsonValue &ledVal, deviceObj["leds"].toArray()) {
                leds.append(static_cast<RazerLedId>(ledVal.toInt()));
            }
            QVector<RazerDeviceQuirks> quirks;
            foreach(const QJsonValue &quirkVal, deviceObj["quirks"].toArray()) {
                quirks.append(static_cast<RazerDeviceQuirks>(quirkVal.toInt()));
            }

            RazerDevice *device = initializeDevice(NULL, vid, pid, deviceObj["name"].toString(), deviceObj["type"].toString(), deviceObj["pclass"].toString(), leds, quirks);
            if(device == NULL)
                continue;

            devices.append(device);

            // D-Bus
            new RazerDeviceAdaptor(device);
            if(!connection.registerObject(device->getObjectPath().path(), device)) {
                qCritical("Failed to register D-Bus object at \"%s\".", qUtf8Printable(device->getObjectPath().path()));
                delete device;
                break;
            }
        }
    }

    DeviceManager *manager = new DeviceManager(devices);
    new DeviceManagerAdaptor(manager);
    if(!connection.registerObject("/io/github/openrazer1", manager)) {
        qCritical("Failed to register D-Bus object at \"/io/github/openrazer1\".");
        return 1;
    }

#ifdef DEMO
    if(devices.isEmpty()) {
        qFatal("No device found. Exiting.");
        return 1;
    }
    RazerDevice *razerDevice = devices[0];

    // Serial
    qDebug() << "Serial:" << razerDevice->getSerial();

    // Firmware version
    qDebug() << "Firmware version:" << razerDevice->getFirmwareVersion();

    foreach(RazerLedId id, razerDevice->getLedIds()) {
        qDebug() << "LED ID:" << static_cast<uchar>(id);
        razerDevice->setStatic(id, 0xFF, 0xFF, 0x00);
    }
    return 0;
#else

    if(!connection.registerService("io.github.openrazer1")) {
        qCritical("Failed to register D-Bus service at \"io.github.openrazer1\". Maybe it's already running?");
        return 1;
    }

    return app.exec();
#endif
}
