#ifndef RAZER_TEST_H
#define RAZER_TEST_H

// LED STATE
#define STATE_OFF 0x00
#define STATE_ON  0x01

// LED STORAGE Options
#define STORE_NOSTORE       0x00
#define STORE_VARSTORE      0x01

// LED definitions
#define LED_SCROLL_WHEEL    0x01
#define LED_BATTERY         0x03
#define LED_LOGO            0x04
#define LED_BACKLIGHT       0x05
#define LED_MACRO           0x07
#define LED_GAME            0x08
#define LED_PROFILE_RED     0x0C
#define LED_PROFILE_GREEN   0x0D
#define LED_PROFILE_BLUE    0x0E

// LED Effect definitions
#define LED_EFFECT_STATIC           0x00
#define LED_EFFECT_BLINKING         0x01
#define LED_EFFECT_PULSATING        0x02
#define LED_EFFECT_SPECTRUM_CYCLING 0x04

// Report Responses
#define RAZER_CMD_BUSY          0x01
#define RAZER_CMD_SUCCESSFUL    0x02
#define RAZER_CMD_FAILURE       0x03
#define RAZER_CMD_TIMEOUT       0x04
#define RAZER_CMD_NOT_SUPPORTED 0x05

#endif
