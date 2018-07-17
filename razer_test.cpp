#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <hidapi.h>

#include "razerdevice.h"
#include "razerreport.h"


int main(int argc, char *argv[])
{
    if (hid_init())
        return -1;

    RazerDevice razerDevice(0x1532, 0x0043);
    if(!razerDevice.openDeviceHandle()) {
        printf("Failed to open device handle\n");
        return -1;
    }

    razer_report report;
    razer_report response_report;
    printf("sizeof(razer_report): %lu\n", sizeof(razer_report)); // should be 91

    report = razer_chroma_standard_get_serial();
    razerDevice.sendReport(report, &response_report);
    printf("Serial: ");
    printf("%s\n", response_report.arguments);

    while (1) {
        report = razer_chroma_standard_set_led_brightness(STORE_VARSTORE, LED_LOGO, 0xFF);
        razerDevice.sendReport(report, &response_report);
        usleep(200000); // 0.2 seconds

        report = razer_chroma_standard_set_led_brightness(STORE_VARSTORE, LED_LOGO, 0x10);
        razerDevice.sendReport(report, &response_report);
        usleep(200000); // 0.2 seconds
    }
}
