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

#include "razerreport.h"

/**
 * @todo write docs
 */
class RazerDevice
{
public:
    /**
     * Default constructor
     */
    RazerDevice(unsigned short vendor_id, unsigned short product_id);
    bool openDeviceHandle();
    int sendReport(razer_report report, razer_report *response_report);

private:
    hid_device *handle;
    unsigned short vendor_id;
    unsigned short product_id;
};

#endif // RAZERDEVICE_H
