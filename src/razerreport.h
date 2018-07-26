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

#include "led/razerled.h"
#include "led/razerclassicled.h"
#include "led/razermatrixled.h"

enum RazerVarstore {
    NOSTORE = 0x00,
    STORE = 0x01
};

enum RazerStatus {
    CMD_NEW = 0x00,
    CMD_BUSY = 0x01,
    CMD_SUCCESSFUL = 0x02,
    CMD_FAILURE = 0x03,
    CMD_NO_RESPONSE_TIMEOUT = 0x04,
    CMD_NOT_SUPPORTED = 0x05
};

struct razer_rgb {
    unsigned char r,g,b;
};


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
    unsigned char status;
    union transaction_id_union transaction_id; /* */
    unsigned short remaining_packets; /* Big Endian */
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

razer_report razer_chroma_standard_set_led_brightness(RazerVarstore variable_storage, RazerLedId led_id, unsigned char brightness);
razer_report razer_chroma_standard_get_led_brightness(RazerVarstore variable_storage, RazerLedId led_id);
razer_report razer_chroma_standard_set_led_effect(RazerVarstore variable_storage, RazerLedId led_id, RazerClassicEffectId led_effect);
razer_report razer_chroma_standard_get_led_effect(RazerVarstore variable_storage, RazerLedId led_id);
razer_report razer_chroma_standard_set_led_rgb(RazerVarstore variable_storage, RazerLedId led_id, unsigned char red, unsigned char green, unsigned char blue);
razer_report razer_chroma_standard_get_led_rgb(RazerVarstore variable_storage, RazerLedId led_id);
razer_report razer_chroma_standard_set_led_state(RazerVarstore variable_storage, RazerLedId led_id, RazerClassicLedState led_state);
razer_report razer_chroma_standard_get_led_state(RazerVarstore variable_storage, RazerLedId led_id);

razer_report razer_chroma_standard_get_firmware_version();
razer_report razer_chroma_standard_get_serial();
razer_report razer_chroma_standard_get_device_mode();

razer_report razer_chroma_standard_matrix_effect(RazerVarstore variable_storage, RazerLedId led_id, RazerMatrixEffectId effect);

#endif // RAZERREPORT_H
