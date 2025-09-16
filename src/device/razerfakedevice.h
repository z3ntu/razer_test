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

#ifndef RAZERFAKEDEVICE_H
#define RAZERFAKEDEVICE_H

#include "razerdevice.h"

/**
 * @todo write docs
 */
class RazerFakeDevice : public RazerDevice
{
    using RazerDevice::RazerDevice;

    bool openDeviceHandle() override;

    bool initialize() override;

    QString getSerial() override;
    QString getFirmwareVersion() override;
    QString getKeyboardLayout() override;

    openrazer::DPI getDPI() override;
    bool setDPI(openrazer::DPI dpi) override;

    ushort getPollRate() override;
    bool setPollRate(ushort poll_rate) override;

    bool displayCustomFrame() override;
    bool defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QVector<openrazer::RGB> rgbData) override;

private:
    QString serial;
    QString fwVersion = "v99.99";

    static int serialCounter;

    openrazer::DPI dpi = { 500, 500 };
    ushort poll_rate = 1000;
};

#endif // RAZERFAKEDEVICE_H
