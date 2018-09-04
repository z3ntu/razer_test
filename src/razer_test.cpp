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
    while (i.hasNext()) {
        QFile f(i.next().absoluteFilePath());
        f.open(QFile::ReadOnly);
        QJsonArray a = QJsonDocument::fromJson(f.readAll()).array();
        foreach (const QJsonValue &value, a) {
            devices.append(value);
        }
    }
    return devices;
}

bool getVidPidFromJson(QJsonObject deviceObj, ushort *vid, ushort *pid)
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

bool getDeviceInfoFromJson(QJsonObject deviceObj, QString *name, QString *type, QString *pclass, QVector<RazerLedId> *leds, QStringList *fx, QStringList *features, QVector<RazerDeviceQuirks> *quirks)
{
    // TODO: Check everything for sanity
    *name = deviceObj["name"].toString();
    *type = deviceObj["type"].toString();
    *pclass = deviceObj["pclass"].toString();
    foreach (const QJsonValue &ledVal, deviceObj["leds"].toArray()) {
        leds->append(static_cast<RazerLedId>(ledVal.toInt()));
    }
    foreach (const QJsonValue &fxVal, deviceObj["fx"].toArray()) {
        fx->append(fxVal.toString());
    }
    foreach (const QJsonValue &featureVal, deviceObj["features"].toArray()) {
        features->append(featureVal.toString());
    }
    foreach (const QJsonValue &quirkVal, deviceObj["quirks"].toArray()) {
        if (quirkVal.toString() == "mouse_matrix") {
            quirks->append(RazerDeviceQuirks::MouseMatrix);
        } else if (quirkVal.toString() == "matrix_brightness") {
            quirks->append(RazerDeviceQuirks::MatrixBrightness);
        } else {
            qCritical("Unhandled quirks string!");
        }
    }
    return true;
}

/**
 * Initializes the RazerDevice object with the data provided.
 * If dev_path is NULL, a fake device is created, otherwise the matching device based on the input JSON is created.
 * Returns NULL on error (error message is printed with qCritical), or a valid RazerDevice*.
 */
RazerDevice *initializeDevice(QString dev_path, QJsonObject deviceObj)
{
    qInfo().noquote().nospace() << "Initializing device: " << deviceObj["name"].toString() << " (" << deviceObj["vid"].toString() << ":" << deviceObj["pid"].toString() << ")";

    ushort vid, pid;
    if (!getVidPidFromJson(deviceObj, &vid, &pid))
        return NULL; // Message is printed in that method

    QString name, type, pclass;
    QVector<RazerLedId> leds;
    QStringList fx;
    QStringList features;
    QVector<RazerDeviceQuirks> quirks;
    if (!getDeviceInfoFromJson(deviceObj, &name, &type, &pclass, &leds, &fx, &features, &quirks)) {
        qCritical("Failed to get device info from JSON");
        return NULL;
    }

    RazerDevice *device;
    if (dev_path == NULL) { // create a fake device
        device = new RazerFakeDevice(dev_path, vid, pid, name, type, pclass, leds, fx, features, quirks);
    } else if (pclass == "classic") {
        device = new RazerClassicDevice(dev_path, vid, pid, name, type, pclass, leds, fx, features, quirks);
    } else if (pclass == "matrix") {
        device = new RazerMatrixDevice(dev_path, vid, pid, name, type, pclass, leds, fx, features, quirks);
    } else {
        qCritical("Unknown device class: %s", qUtf8Printable(pclass));
        return NULL;
    }
    if (!device->openDeviceHandle()) {
        qCritical("Failed to open device handle");
        delete device;
        return NULL;
    }
    if (!device->initializeLeds()) {
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

    // Setup up the command line parser
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption({"fake-devices", "Adds fake devices instead of real ones."});
    parser.process(app);

    qInfo("razer_test - version %s", RAZER_TEST_VERSION);

    // Register the enums with the Qt system
    qRegisterMetaType<RazerLedId>("RazerLedId");
    qDBusRegisterMetaType<RazerLedId>();
    // Register QVector<RazerLedId> as well as otherwise getLedIds() returns nothing.
    qDBusRegisterMetaType<QVector<RazerLedId>>();

    qRegisterMetaType<WaveDirection>("WaveDirection");
    qDBusRegisterMetaType<WaveDirection>();

    qRegisterMetaType<RazerDPI>("RazerDPI");
    qDBusRegisterMetaType<RazerDPI>();

    qRegisterMetaType<ReactiveSpeed>("ReactiveSpeed");
    qDBusRegisterMetaType<ReactiveSpeed>();

    // Get the D-Bus session bus
    QDBusConnection connection = QDBusConnection::sessionBus();

    // Load the supported devices from the json files
    QJsonArray supportedDevices = loadDevicesFromJson();
    if (supportedDevices.isEmpty()) {
        qCritical("JSON device definition files were not found. Exiting.");
        return -1;
    }

    QVector<RazerDevice *> devices;

    // Use the real devices
    if (!parser.isSet("fake-devices")) {
        if (hid_init())
            return -1;

        struct hid_device_info *devs, *cur_dev;
        devs = hid_enumerate(0x1532, 0x0000);
        cur_dev = devs;

        QVector<ushort> devicesPid;

        while (cur_dev) {
            // TODO maybe needs https://github.com/cyanogen/uchroma/blob/2b8485e5ac931980bacb125b8dff7b9a39ea527f/uchroma/server/device_manager.py#L141-L155

            // Check if device is already added
            if (devicesPid.contains(cur_dev->product_id)) {
                cur_dev = cur_dev->next;
                continue;
            }

            // Check if device is supported
            foreach (const QJsonValue &deviceVal, supportedDevices) {
                QJsonObject deviceObj = deviceVal.toObject();

                ushort vid, pid;
                if (!getVidPidFromJson(deviceObj, &vid, &pid))
                    break;

                if (cur_dev->vendor_id == vid && cur_dev->product_id == pid) {
                    RazerDevice *device = initializeDevice(QString(cur_dev->path), deviceObj);
                    if (device == NULL)
                        break;

                    devices.append(device);
                    devicesPid.append(cur_dev->product_id);

                    // D-Bus
                    new RazerDeviceAdaptor(device);
                    if (!connection.registerObject(device->getObjectPath().path(), device)) {
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
    } else { // Handle fake devices
        // Check if device is supported
        foreach (const QJsonValue &deviceVal, supportedDevices) {
            QJsonObject deviceObj = deviceVal.toObject();

            RazerDevice *device = initializeDevice(NULL, deviceObj);
            if (device == NULL)
                continue;

            devices.append(device);

            // D-Bus
            new RazerDeviceAdaptor(device);
            if (!connection.registerObject(device->getObjectPath().path(), device)) {
                qCritical("Failed to register D-Bus object at \"%s\".", qUtf8Printable(device->getObjectPath().path()));
                delete device;
                continue;
            }
        }
    }

    DeviceManager *manager = new DeviceManager(devices);
    new DeviceManagerAdaptor(manager);
    if (!connection.registerObject("/io/github/openrazer1", manager)) {
        qCritical("Failed to register D-Bus object at \"/io/github/openrazer1\".");
        return 1;
    }

#ifdef DEMO

    if (devices.isEmpty()) {
        qFatal("No device found. Exiting.");
        return 1;
    }
    RazerDevice *razerDevice = devices[0];

    // Serial
    qDebug() << "Serial:" << razerDevice->getSerial();

    // Firmware version
    qDebug() << "Firmware version:" << razerDevice->getFirmwareVersion();

    foreach (RazerLedId id, razerDevice->getLedIds()) {
        qDebug() << "LED ID:" << static_cast<uchar>(id);
        razerDevice->setStatic(id, 0xFF, 0xFF, 0x00);
    }
    return 0;

#else

    if (!connection.registerService("io.github.openrazer1")) {
        qCritical("Failed to register D-Bus service at \"io.github.openrazer1\". Maybe it's already running?");
        return 1;
    }

    return app.exec();

#endif
}
