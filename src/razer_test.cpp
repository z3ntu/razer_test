#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <hidapi.h>

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

#include "device/razerdevice.h"
#include "device/razerclassicdevice.h"
#include "device/razermatrixdevice.h"
#include "led/razerled.h"
#include "led/razerclassicled.h"
#include "razerreport.h"


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
    if (hid_init())
        return -1;

    printf("sizeof(razer_report): %lu\n", sizeof(razer_report)); // should be 90

    QJsonArray supportedDevices = loadDevicesFromJson();

    QList<RazerDevice*> devices;
    QList<unsigned short> devicesPid;

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
            unsigned short vid = deviceObj.value("vid").toString().toUShort(&ok, 16);
            if(!ok) {
                qDebug() << "Error converting vid: " << deviceObj.value("vid");
                continue;
            }
            unsigned short pid = deviceObj.value("pid").toString().toUShort(&ok, 16);
            if(!ok) {
                qDebug() << "Error converting pid: " << deviceObj.value("pid");
                continue;
            }

            if(cur_dev->vendor_id == vid && cur_dev->product_id == pid) {
                qDebug() << "Found device!" << deviceObj.value("name").toString() << deviceObj.value("vid").toString() << deviceObj.value("pid").toString();
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

                RazerDevice *device;
                if(pclass == "classic") {
                    device = new RazerClassicDevice(QString(cur_dev->path), cur_dev->vendor_id, cur_dev->product_id, name, type, pclass, leds, quirks);
                } else if(pclass == "matrix") {
                    device = new RazerMatrixDevice(QString(cur_dev->path), cur_dev->vendor_id, cur_dev->product_id, name, type, pclass, leds, quirks);
                } else {
                    qDebug() << "Unknown device class" << pclass;
                    return -2;
                }
                if(!device->openDeviceHandle()) {
                    qDebug() << "Failed to open device handle";
                    return -1;
                }
                devices.append(device);
                devicesPid.append(cur_dev->product_id);
                break;
            }
        }
        cur_dev = cur_dev->next;
    }

    // Free devs and cur_dev pointers
    hid_free_enumeration(devs);

    if(devices.isEmpty()) {
        qDebug() << "No device found. Exiting.";
        return 1;
    }

    // TODO: Handle multiple devices
    RazerDevice *razerDevice = devices[0];

    razer_report report, response_report;

    // Serial
    qDebug() << "Serial:" << razerDevice->getSerial();

    // Firmware version
    qDebug() << "Firmware version:" << razerDevice->getFirmwareVersion();

    // LED ID List
    report = get_razer_report(0x03, 0x89, 0x16); // LED ID List
    razerDevice->sendReport(report, &response_report);
    printf("LED ID List: ");
    for (int i = 0; i < 0x16; i++)
        printf("%02hhx ", response_report.arguments[i]);
    printf("\n");

    foreach(RazerLedId id, razerDevice->getLedIds()) {
        qDebug() << "LED ID:" << id;
        razerDevice->setStatic(id, 0x00, 0xFF, 0xFF);
    }

    // Set state to on
    report = razer_chroma_standard_set_led_state(RazerVarstore::STORE, RazerLedId::LogoLED, RazerClassicLedState::On);
    razerDevice->sendReport(report, &response_report);
    usleep(200000); // 0.2 seconds

    return 0;

    while (1) {
        report = razer_chroma_standard_set_led_brightness(RazerVarstore::STORE, RazerLedId::LogoLED, 0xFF);
        razerDevice->sendReport(report, &response_report);
        usleep(200000); // 0.2 seconds

        report = razer_chroma_standard_set_led_brightness(RazerVarstore::STORE, RazerLedId::LogoLED, 0x10);
        razerDevice->sendReport(report, &response_report);
        usleep(200000); // 0.2 seconds
    }
}
