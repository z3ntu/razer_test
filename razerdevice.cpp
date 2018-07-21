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

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "razerdevice.h"

RazerDevice::RazerDevice(unsigned short vendor_id, unsigned short product_id)
{
    this->vendor_id = vendor_id;
    this->product_id = product_id;
}

bool RazerDevice::openDeviceHandle()
{
    handle = hid_open(vendor_id, product_id, NULL);
    if (!handle) {
        printf("unable to open device\n");
        return false;
    }
    return true;
}

int RazerDevice::sendReport(razer_report report, razer_report *response_report)
{
    int res;
    unsigned char buf[91];

    // Calculate the crc
    report.crc = razer_calculate_crc(&report);

    res = hid_send_feature_report(handle, reinterpret_cast<const unsigned char*>(&report), 91);
    if (res < 0) {
        printf("Unable to send a feature report.\n");
        return 1;
    }

    // Wait a bit
//     usleep(600);
    usleep(800);

    // Read a Feature Report from the device
    buf[0] = 0x00; // report id
    res = hid_get_feature_report(handle, buf, sizeof(buf));
    if (res < 0) {
        printf("Unable to get a feature report.\n");
        return 2;
    }
    // Print out the returned buffer.
//         printf("Feature Report\n   ");
//         for (i = 0; i < res; i++)
//             printf("%02hhx ", buf[i]);
//         printf("\n");

    memcpy(response_report, buf, sizeof(buf));
    printf("Response report: Status: %02x transaction id: %02x Data size: %02x Command class: %02x Command id: %02x\n",
           response_report->status,
           response_report->transaction_id.id,
           response_report->data_size,
           response_report->command_class,
           response_report->command_id.id);
    return 0;
}
