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

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qInfo("razer_test - version %s", RAZER_TEST_VERSION);

    qRegisterMetaType<RazerLedId>("RazerLedId");
    qDBusRegisterMetaType<RazerLedId>();

    qRegisterMetaType<WaveDirection>("WaveDirection");
    qDBusRegisterMetaType<WaveDirection>();

    QDBusConnection connection = QDBusConnection::sessionBus();

    if (hid_init())
        return -1;

    printf("sizeof(razer_report): %lu\n", sizeof(razer_report)); // should be 90

    QJsonArray supportedDevices = loadDevicesFromJson();

    QVector<RazerDevice*> devices;
    QVector<ushort> devicesPid;

    struct hid_device_info *devs, *cur_dev;
    devs = hid_enumerate(0x1532, 0x0000);
    cur_dev = devs;
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
                QString pclass = deviceObj["pclass"].toString();
                QString name = deviceObj["name"].toString();
                QString type = deviceObj["type"].toString();
                QVector<RazerLedId> leds;
                foreach(const QJsonValue &ledVal, deviceObj["leds"].toArray()) {
                    leds.append(static_cast<RazerLedId>(ledVal.toInt()));
                }
                QVector<RazerDeviceQuirks> quirks;
                foreach(const QJsonValue &quirkVal, deviceObj["quirks"].toArray()) {
                    quirks.append(static_cast<RazerDeviceQuirks>(quirkVal.toInt()));
                }

                RazerDevice *device = NULL;
                if(pclass == "classic") {
                    device = new RazerClassicDevice(QString(cur_dev->path), cur_dev->vendor_id, cur_dev->product_id, name, type, pclass, leds, quirks);
                } else if(pclass == "matrix") {
                    device = new RazerMatrixDevice(QString(cur_dev->path), cur_dev->vendor_id, cur_dev->product_id, name, type, pclass, leds, quirks);
                } else {
                    qCritical("Unknown device class: %s", qUtf8Printable(pclass));
                    break;
                }
                if(!device->openDeviceHandle()) {
                    qCritical("ERROR: Failed to open device handle");
                    delete device;
                    break;
                }
                if(!device->initializeLeds()) {
                    qCritical("ERROR: Failed to initialize leds");
                    delete device;
                    break;
                }
                devices.append(device);
                devicesPid.append(cur_dev->product_id);

                // D-Bus
                new RazerDeviceAdaptor(device);
                connection.registerObject(device->getObjectPath().path(), device);

                break;
            }
        }
        cur_dev = cur_dev->next;
    }

    // Free devs and cur_dev pointers
    hid_free_enumeration(devs);

    DeviceManager *manager = new DeviceManager(devices);
    new DeviceManagerAdaptor(manager);
    connection.registerObject("/io/github/openrazer1", manager);

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
        qDebug() << "LED ID:" << id;
        razerDevice->setStatic(id, 0xFF, 0xFF, 0x00);
    }
    return 0;
#else

    connection.registerService("io.github.openrazer1");

    return app.exec();
#endif
}
