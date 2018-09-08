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
#include <QByteArray>

#include "../razer_test.h"
#include "../razerreport.h"
#include "../customeffect/customeffectthread.h"
#include "../led/razerled.h"

// class RazerLED;

/**
 * @todo write docs
 */
class RazerDevice : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "io.github.openrazer1.Device")
    Q_PROPERTY(QString Name READ getName)
    Q_PROPERTY(QString Type READ getType)
    Q_PROPERTY(QList<QDBusObjectPath> Leds READ getLedObjectPaths)
    Q_PROPERTY(QStringList SupportedFx READ getSupportedFx)
    Q_PROPERTY(QStringList SupportedFeatures READ getSupportedFeatures)
    Q_PROPERTY(MatrixDimensions MatrixDimensions READ getMatrixDimensions)

public:
    RazerDevice(QString dev_path, ushort vendor_id, ushort product_id, QString name, QString type, QString pclass, QVector<RazerLedId> ledIds, QStringList fx, QStringList features, QVector<RazerDeviceQuirks> quirks, MatrixDimensions matrixDimensions, ushort maxDPI);
    virtual ~RazerDevice();

    virtual bool openDeviceHandle();
    virtual bool initializeLeds() = 0;

    int sendReport(razer_report request_report, razer_report *response_report);
    QDBusObjectPath getObjectPath();

    // Getters behind properties (Q_PROPERTY)
    QString getName();
    QString getType();
    QStringList getSupportedFx();
    QStringList getSupportedFeatures();
    MatrixDimensions getMatrixDimensions();

    QHash<RazerLedId, RazerLED *> getLeds();
    QList<QDBusObjectPath> getLedObjectPaths();

    // TODO: Make protected again
    QVector<RazerDeviceQuirks> quirks;

    // TODO: Deprecate this method, checkFx is better
    bool checkLedAndFx(RazerLedId led, QString fxStr);
    bool checkFx(QString fxStr);

public Q_SLOTS:
    // TODO: CamelCase public functions (at least for D-Bus)
    virtual QString getSerial();
    virtual QString getFirmwareVersion();
    virtual QString getKeyboardLayout();

    virtual RazerDPI getDPI();
    virtual bool setDPI(RazerDPI dpi);
    ushort getMaxDPI();

    virtual ushort getPollRate();
    virtual bool setPollRate(ushort poll_rate);

    // Custom frame
    virtual bool displayCustomFrame() = 0;
    virtual bool defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QByteArray rgbData) = 0;

    // getDeviceMode, setDeviceMode

    bool startCustomEffectThread(QString effectName);
    void pauseCustomEffectThread();

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

    MatrixDimensions matrixDimensions;
    ushort maxDPI;

    CustomEffectThread thread;

    QHash<RazerLedId, RazerLED *> leds;

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

private slots:
    void customRgbDataReady(uchar row, uchar startColumn, uchar endColumn, const QByteArray &rgbData);
    void customFrameReady();
};

#endif // RAZERDEVICE_H
