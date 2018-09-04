/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018  Luca Weiss <luca@z3ntu.xyz>
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

#ifndef RAZERDEVICE_H
#define RAZERDEVICE_H

#include <hidapi.h>

#include <QObject>
#include <QString>
#include <QVector>
#include <QHash>
#include <QDBusContext>

#include "../razerreport.h"

enum class RazerDeviceQuirks {
    MouseMatrix,
    MatrixBrightness
};

struct RazerDPI {
    ushort dpi_x;
    ushort dpi_y;
};
Q_DECLARE_METATYPE(RazerDPI)

QDBusArgument &operator<<(QDBusArgument &argument, const RazerDPI &razerDPI);
const QDBusArgument &operator>>(const QDBusArgument &argument, RazerDPI &razerDPI);

/**
 * @todo write docs
 */
class RazerDevice : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "io.github.openrazer1.Device")
    Q_PROPERTY(QString Name READ getName)
    Q_PROPERTY(QString Type READ getType)
    Q_PROPERTY(QVector<RazerLedId> LedIds READ getLedIds)
    Q_PROPERTY(QStringList SupportedFx READ getSupportedFx)
    Q_PROPERTY(QStringList SupportedFeatures READ getSupportedFeatures)

public:
    RazerDevice(QString dev_path, ushort vendor_id, ushort product_id, QString name, QString type, QString pclass, QVector<RazerLedId> ledIds, QStringList fx, QStringList features, QVector<RazerDeviceQuirks> quirks);
    virtual ~RazerDevice();
    virtual bool openDeviceHandle();
    int sendReport(razer_report request_report, razer_report *response_report);
    QDBusObjectPath getObjectPath();

    virtual bool initializeLeds() = 0;
    virtual bool getBrightness(RazerLedId led, uchar *brightness) = 0;

    QString getName();
    QString getType();

    QVector<RazerLedId> getLedIds();
    QStringList getSupportedFx();
    QStringList getSupportedFeatures();

public Q_SLOTS:
    // TODO: CamelCase public functions (at least for D-Bus)
    virtual QString getSerial();
    virtual QString getFirmwareVersion();
    virtual QString getKeyboardLayout();

    virtual RazerDPI getDPI();
    virtual bool setDPI(RazerDPI dpi);

    virtual ushort getPollRate();
    virtual bool setPollRate(ushort poll_rate);

    // FX
    virtual bool setNone(RazerLedId led) = 0;
    virtual bool setStatic(RazerLedId led, uchar red, uchar green, uchar blue) = 0;
    virtual bool setBreathing(RazerLedId led, uchar red, uchar green, uchar blue) = 0;
    virtual bool setBreathingDual(RazerLedId led, uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2) = 0;
    virtual bool setBreathingRandom(RazerLedId led) = 0;
    virtual bool setBlinking(RazerLedId led, uchar red, uchar green, uchar blue) = 0;
    virtual bool setSpectrum(RazerLedId led) = 0;
    virtual bool setWave(RazerLedId led, WaveDirection direction) = 0;
    virtual bool setCustomFrame(RazerLedId led) = 0;
//     virtual bool defineCustomRgb( // TODO
    // etc

    // getDeviceMode, setDeviceMode


    virtual bool setBrightness(RazerLedId led, uchar brightness) = 0;
    uchar getBrightness(RazerLedId led);

protected:
    hid_device *handle = NULL;

    QString dev_path;
    ushort vendor_id;
    ushort product_id;
    QString name;
    QString type;
    QString pclass;
    QVector<RazerLedId> ledIds;
    QStringList fx;
    QStringList features;
    QVector<RazerDeviceQuirks> quirks;

    QHash<RazerLedId, RazerLED *> leds;

    bool checkLedAndFx(RazerLedId led, QString fxStr);
    bool checkFeature(QString featureStr);

    QHash<uchar, QString> keyboardLayoutIds {
        {0x01, "US"},
        {0x02, "Greek"},
        {0x03, "German"},
        {0x04, "French"},
        {0x06, "UK"},
        {0x07, "Nordic"},
        {0x11, "Italian"},
        {0x12, "Portugese"},
        {0x81, "US-mac"}
    };
};

#endif // RAZERDEVICE_H
