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

#ifndef RAZERREPORT_H
#define RAZERREPORT_H

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

union transaction_id_union {
    unsigned char id;
    struct transaction_parts {
        unsigned char device : 3;
        unsigned char id : 5;
    } parts;
};

union command_id_union {
    unsigned char id;
    struct command_id_parts {
        unsigned char direction : 1;
        unsigned char id : 7;
    } parts;
};

typedef struct razer_report {
    unsigned char report_id; /* required by hidraw or something - 0x00 */
    unsigned char status;
    union transaction_id_union transaction_id; /* */
    unsigned char remaining_packets; /* Big Endian */
    unsigned char remaining_packets2; /* should be unsigned short - but breaks stuff */
    unsigned char protocol_type; /*0x0*/
    unsigned char data_size;
    unsigned char command_class;
    union command_id_union command_id;
    unsigned char arguments[80];
    unsigned char crc;/*xor'ed bytes of report*/
    unsigned char reserved; /*0x0*/
} razer_report;

unsigned char razer_calculate_crc(razer_report *report);
razer_report get_razer_report(unsigned char command_class, unsigned char command_id, unsigned char data_size);
razer_report razer_chroma_standard_set_led_brightness(unsigned char variable_storage, unsigned char led_id, unsigned char brightness);
razer_report razer_chroma_standard_get_serial();

#endif // RAZERREPORT_H
