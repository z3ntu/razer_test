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

#include "devicemanager.h"

#include "config.h"

DeviceManager::DeviceManager(QVector<RazerDevice *> rDevices)
{
    setDevices(rDevices);
}

QString DeviceManager::getVersion()
{
    return RAZER_TEST_VERSION;
}

QList<QDBusObjectPath> DeviceManager::getDevices()
{
    return devices.toList();
}

QDBusObjectPath DeviceManager::getObjectPath()
{
    return QDBusObjectPath("/io/github/openrazer1");
}

void DeviceManager::setDevices(QVector<RazerDevice *> rDevices)
{
    devices.clear();
    foreach (RazerDevice *rDevice, rDevices) {
        devices.append(rDevice->getObjectPath());
    }
    emit devicesChanged();
}
