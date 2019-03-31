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

#ifndef RAZERREPORT_H
#define RAZERREPORT_H

#include "razer_test.h"
#include "razer_test_private.h"

using namespace razer_test;

struct razer_report_struct {
    uchar status;

    union {
        uchar transaction_id;
        struct {
            uchar device : 3;
            uchar id : 5;
        } transaction_id_parts;
    };

    ushort remaining_packets;
    uchar protocol_type;
    uchar data_size;
    uchar command_class;

    union {
        uchar command_id;
        struct {
            uchar direction : 1;
            uchar id : 7;
        } command_id_parts;
    };

    uchar arguments[80];
    uchar crc;
    uchar reserved;
};
using razer_report = razer_report_struct;


uchar razer_calculate_crc(razer_report *report);
razer_report get_razer_report(uchar command_class, uchar command_id, uchar data_size);

razer_report razer_chroma_standard_set_led_brightness(RazerVarstore variable_storage, RazerLedId led_id, uchar brightness);
razer_report razer_chroma_standard_get_led_brightness(RazerVarstore variable_storage, RazerLedId led_id);
razer_report razer_chroma_standard_set_led_effect(RazerVarstore variable_storage, RazerLedId led_id, RazerClassicEffectId led_effect);
razer_report razer_chroma_standard_get_led_effect(RazerVarstore variable_storage, RazerLedId led_id);
razer_report razer_chroma_standard_set_led_rgb(RazerVarstore variable_storage, RazerLedId led_id, uchar red, uchar green, uchar blue);
razer_report razer_chroma_standard_get_led_rgb(RazerVarstore variable_storage, RazerLedId led_id);
razer_report razer_chroma_standard_set_led_state(RazerVarstore variable_storage, RazerLedId led_id, RazerClassicLedState led_state);
razer_report razer_chroma_standard_get_led_state(RazerVarstore variable_storage, RazerLedId led_id);

razer_report razer_chroma_standard_get_firmware_version();
razer_report razer_chroma_standard_get_serial();
razer_report razer_chroma_standard_get_device_mode();
razer_report razer_chroma_standard_get_keyboard_layout();

razer_report razer_chroma_standard_matrix_effect(RazerMatrixEffectId effect);
razer_report razer_chroma_standard_matrix_set_custom_frame(uchar row_index, uchar start_col, uchar stop_col, const uchar *rgb_data);
razer_report razer_chroma_extended_mouse_matrix_effect(RazerVarstore variable_storage, RazerLedId led_id, RazerMouseMatrixEffectId effect);
razer_report razer_chroma_extended_matrix_set_brightness(RazerVarstore variable_storage, RazerLedId led_id, uchar brightness);
razer_report razer_chroma_extended_matrix_get_brightness(RazerVarstore variable_storage, RazerLedId led_id);

razer_report razer_chroma_misc_one_row_set_custom_frame(uchar start_col, uchar stop_col, const uchar *rgb_data);
razer_report razer_chroma_misc_set_dpi_xy(RazerVarstore variable_storage, ushort dpi_x, ushort dpi_y);
razer_report razer_chroma_misc_get_dpi_xy(RazerVarstore variable_storage);

razer_report razer_chroma_misc_get_polling_rate();
razer_report razer_chroma_misc_set_polling_rate(uchar poll_rate_byte);

#endif // RAZERREPORT_H
