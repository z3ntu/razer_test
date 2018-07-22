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

int RazerDevice::sendReport(razer_report request_report, razer_report *response_report)
{
    int res;
    unsigned char req_buf[sizeof(razer_report)+1];
    unsigned char res_buf[sizeof(razer_report)+1];

    // Calculate the crc
    request_report.crc = razer_calculate_crc(&request_report);

    // Copy request_report into req_buf, shifted by 1 byte to the right for the report number
    req_buf[0] = 0x00; // report number
    memcpy(&req_buf[1], &request_report, sizeof(razer_report));

#ifdef DEBUG
    printf("Request report: ");
    for (int i = 1; i < sizeof(razer_report)+1; i++)
        printf("%02hhx ", req_buf[i]);
    printf("\n");
#endif

    // TODO: Retry feature report if it fails

    // Send the Feature Report to the device
    res = hid_send_feature_report(handle, req_buf, sizeof(req_buf));
    if (res < 0) {
        printf("Unable to send a feature report.\n");
        return 1;
    }

    // Wait a bit
//     usleep(600);
    usleep(800);

    // Read a Feature Report from the device
    res_buf[0] = 0x00; // report number
    res = hid_get_feature_report(handle, res_buf, sizeof(res_buf));
    if (res < 0) {
        printf("Unable to get a feature report.\n");
        return 2;
    }

#ifdef DEBUG
    printf("Response report: ");
    for (int i = 1; i < sizeof(razer_report)+1; i++)
        printf("%02hhx ", res_buf[i]);
    printf("\n");
#endif

    // Copy returned data into the response_report, minus the report number
    memcpy(response_report, &res_buf[1], sizeof(razer_report));

    printf("Response report: Status: %02x transaction id: %02x Data size: %02x Command class: %02x Command id: %02x\n",
           response_report->status,
           response_report->transaction_id.id,
           response_report->data_size,
           response_report->command_class,
           response_report->command_id.id);
    return 0;
}
