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

#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QVector>
#include <QDBusObjectPath>

#include "../device/razerdevice.h"

/**
 * @todo write docs
 */
class DeviceManager : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "io.github.openrazer1.Manager")
    Q_PROPERTY(QList<QDBusObjectPath> Devices READ getDevices)
    Q_PROPERTY(QString Version READ getVersion)

public:
    DeviceManager(QVector<RazerDevice *> devices);

    QString getVersion();
    QList<QDBusObjectPath> getDevices();
    QDBusObjectPath getObjectPath();

    void setDevices(QVector<RazerDevice *> devices);

signals:
    void devicesChanged();

private:
    QVector<QDBusObjectPath> devices;
};

#endif // DEVICEMANAGER_H
