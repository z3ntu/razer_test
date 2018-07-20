#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <hidapi.h>

#include "razerdevice.h"
#include "razerreport.h"
#include "razerled.h"


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

    razer_report response_report2;
    memset(&response_report2, 0, sizeof(razer_report));
    report = get_razer_report(0x03, 0x89, 0x16); // LED ID List
    razerDevice.sendReport(report, &response_report2);
    printf("LED ID List: ");
    for (int i = 0; i < 0x16; i++)
        printf("%02hhx ", response_report2.arguments[i]);
    printf("\n");
    return 0;

    struct razer_rgb rgb;
    rgb.r = 0xFF;
    rgb.g = 0x00;
    rgb.b = 0xFF;
    report = razer_chroma_standard_set_led_rgb(RazerVarstore::STORE, RazerLedId::LogoLED, &rgb);
    razerDevice.sendReport(report, &response_report);
    usleep(200000); // 0.2 seconds

    report = razer_chroma_standard_set_led_effect(RazerVarstore::STORE, RazerLedId::LogoLED, RazerEffectId::Static);
    razerDevice.sendReport(report, &response_report);
    usleep(200000); // 0.2 seconds

    report = razer_chroma_standard_set_led_state(RazerVarstore::STORE, RazerLedId::LogoLED, RazerLedState::On);
    razerDevice.sendReport(report, &response_report);
    usleep(200000); // 0.2 seconds

    return 0;

    while (1) {
        report = razer_chroma_standard_set_led_brightness(RazerVarstore::STORE, RazerLedId::LogoLED, 0xFF);
        razerDevice.sendReport(report, &response_report);
        usleep(200000); // 0.2 seconds

        report = razer_chroma_standard_set_led_brightness(RazerVarstore::STORE, RazerLedId::LogoLED, 0x10);
        razerDevice.sendReport(report, &response_report);
        usleep(200000); // 0.2 seconds
    }
}
