/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2019  Luca Weiss <luca@z3ntu.xyz>
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

#ifndef DAEMON_H
#define DAEMON_H

#include "device/razerdevice.h"

#include <QDBusConnection>
#include <QJsonArray>
#include <QObject>
#if defined(Q_OS_LINUX)
#include "devnotifier/devicenotifierlinux.h"
#elif defined(Q_OS_DARWIN)
#include "devnotifier/devicenotifiermac.h"
#else
#define NO_DEVNOTIFIER_IMPL
#include "devnotifier/idevicenotifier.h"
#endif
#include "manager/devicemanager.h"

/**
 * @todo write docs
 */
class Daemon : public QObject
{
    Q_OBJECT
public:
    Daemon(bool develMode, bool fakeDevices);

    bool initialize();
    void discoverDevices();
    void discoverFakeDevices();
    RazerDevice *initializeDevice(QString dev_path, QJsonObject deviceObj);
    QJsonArray loadDevicesFromJson(bool devel);
    bool getVidPidFromJson(QJsonObject deviceObj, ushort *vid, ushort *pid);
    bool registerDeviceOnDBus(RazerDevice *device);
    void unregisterDeviceOnDBus(RazerDevice *device);
    bool getDeviceInfoFromJson(QJsonObject deviceObj, QString *name, QString *type, QString *pclass, QVector<RazerLedId> *leds, QStringList *fx, QStringList *features, QVector<RazerDeviceQuirks> *quirks, MatrixDimensions *matrixDimensions, ushort *maxDPI);
    QJsonObject getDeviceJsonForDevice(ushort vid, ushort pid);

    static QString hexUshortToString(ushort number);

public slots:
    void rediscoverDevices();

signals:
    void devicesChanged(QVector<RazerDevice *> devices);

private:
    QVector<RazerDevice *> devices;
    QHash<QString, RazerDevice *> devicesHash;

    IDeviceNotifier *notifier = nullptr;
    DeviceManager *manager;

    QStringList devicePathList;
    QJsonArray supportedDevices;
    QDBusConnection connection;

    bool develMode;
    bool fakeDevices;
};

#endif // DAEMON_H
