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

#include "razerreport.h"

#include <cstring>

/**
 * Calculate the checksum for the usb message
 *
 * Checksum byte is stored in the 2nd last byte in the messages payload.
 * The checksum is generated by XORing all the bytes in the report starting
 * at byte number 2 (0 based) and ending at byte 88.
 */
uchar razer_calculate_crc(razer_report *report)
{
    /*second to last byte of report is a simple checksum*/
    /*just xor all bytes up with overflow and you are done*/
    uchar crc = 0;
    auto *_report = (uchar *)report;

    unsigned int i;
    for (i = 3; i < 89; i++) {
        crc ^= _report[i];
    }

    return crc;
}

razer_report get_razer_report(uchar command_class, uchar command_id, uchar data_size)
{
    razer_report new_report;
    memset(&new_report, 0, sizeof(razer_report));

    new_report.status = 0x00;
    new_report.transaction_id = 0xFF;
    new_report.remaining_packets = 0x00;
    new_report.protocol_type = 0x00;
    new_report.command_class = command_class;
    new_report.command_id = command_id;
    new_report.data_size = data_size;

    return new_report;
}

razer_report razer_chroma_standard_set_led_brightness(RazerVarstore variable_storage, RazerLedId led_id, uchar brightness)
{
    razer_report report = get_razer_report(0x03, 0x03, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    report.arguments[2] = brightness;

    return report;
}

razer_report razer_chroma_standard_get_led_brightness(RazerVarstore variable_storage, RazerLedId led_id)
{
    razer_report report = get_razer_report(0x03, 0x83, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);

    return report;
}

razer_report razer_chroma_standard_set_led_effect(RazerVarstore variable_storage, RazerLedId led_id, RazerClassicEffectId led_effect)
{
    razer_report report = get_razer_report(0x03, 0x02, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    report.arguments[2] = static_cast<uchar>(led_effect);

    return report;
}

razer_report razer_chroma_standard_get_led_effect(RazerVarstore variable_storage, RazerLedId led_id)
{
    razer_report report = get_razer_report(0x03, 0x82, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);

    return report;
}

razer_report razer_chroma_standard_set_led_rgb(RazerVarstore variable_storage, RazerLedId led_id, uchar red, uchar green, uchar blue)
{
    razer_report report = get_razer_report(0x03, 0x01, 0x05);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    report.arguments[2] = red;
    report.arguments[3] = green;
    report.arguments[4] = blue;

    return report;
}

razer_report razer_chroma_standard_get_led_rgb(RazerVarstore variable_storage, RazerLedId led_id)
{
    razer_report report = get_razer_report(0x03, 0x81, 0x05);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    return report;
}

razer_report razer_chroma_standard_set_led_state(RazerVarstore variable_storage, RazerLedId led_id, RazerClassicLedState led_state)
{
    razer_report report = get_razer_report(0x03, 0x00, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    report.arguments[2] = static_cast<uchar>(led_state);

    return report;
}

razer_report razer_chroma_standard_get_led_state(RazerVarstore variable_storage, RazerLedId led_id)
{
    razer_report report = get_razer_report(0x03, 0x80, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);

    return report;
}

razer_report razer_chroma_standard_get_firmware_version()
{
    return get_razer_report(0x00, 0x81, 0x02);
}

razer_report razer_chroma_standard_get_serial()
{
    return get_razer_report(0x00, 0x82, 0x16);
}

razer_report razer_chroma_standard_get_device_mode()
{
    return get_razer_report(0x00, 0x84, 0x02);
}

razer_report razer_chroma_standard_get_keyboard_layout()
{
    return get_razer_report(0x00, 0x86, 0x02);
}

razer_report razer_chroma_standard_matrix_effect(RazerMatrixEffectId effect)
{
    razer_report report = get_razer_report(0x03, 0x0A, 80);
    report.arguments[0] = static_cast<uchar>(effect); // Effect ID

    return report;
}

razer_report razer_chroma_standard_matrix_set_custom_frame(uchar row_index, uchar start_col, uchar stop_col, const uchar *rgb_data)
{
    razer_report report = get_razer_report(0x03, 0x0B, 0x46);
    auto row_length = (size_t)(((stop_col + 1) - start_col) * 3);

    report.arguments[0] = 0xFF; // Frame ID
    report.arguments[1] = row_index;
    report.arguments[2] = start_col;
    report.arguments[3] = stop_col;
    memcpy(&report.arguments[4], rgb_data, row_length);

    return report;
}

razer_report razer_chroma_extended_mouse_matrix_effect(RazerVarstore variable_storage, RazerLedId led_id, RazerMouseMatrixEffectId effect)
{
    razer_report report = get_razer_report(0x0F, 0x02, 80);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    report.arguments[2] = static_cast<uchar>(effect);

    return report;
}

razer_report razer_chroma_extended_matrix_set_brightness(RazerVarstore variable_storage, RazerLedId led_id, uchar brightness)
{
    razer_report report = get_razer_report(0x0F, 0x04, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    report.arguments[2] = brightness;

    return report;
}

razer_report razer_chroma_extended_matrix_get_brightness(RazerVarstore variable_storage, RazerLedId led_id)
{
    razer_report report = get_razer_report(0x0F, 0x84, 0x03);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);

    return report;
}

razer_report razer_chroma_extended_matrix_effect(RazerVarstore variable_storage, RazerLedId led_id, RazerMouseMatrixEffectId effect_id)
{
    razer_report report = get_razer_report(0x0F, 0x02, 80);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = static_cast<uchar>(led_id);
    report.arguments[2] = static_cast<uchar>(effect_id);

    return report;
}

razer_report razer_chroma_extended_matrix_set_custom_frame(uchar row_index, uchar start_col, uchar stop_col, const uchar *rgb_data)
{
    razer_report report = get_razer_report(0x0F, 0x03, 0x47);
    auto row_length = (size_t)(((stop_col + 1) - start_col) * 3);

    report.arguments[2] = row_index;
    report.arguments[3] = start_col;
    report.arguments[4] = stop_col;
    memcpy(&report.arguments[5], rgb_data, row_length);

    return report;
}

razer_report razer_chroma_misc_one_row_set_custom_frame(uchar start_col, uchar stop_col, const uchar *rgb_data)
{
    razer_report report = get_razer_report(0x03, 0x0C, 0x32);
    auto row_length = (size_t)(((stop_col + 1) - start_col) * 3);

    report.arguments[0] = start_col;
    report.arguments[1] = stop_col;
    memcpy(&report.arguments[2], rgb_data, row_length);

    return report;
}

razer_report razer_chroma_misc_set_dpi_xy(RazerVarstore variable_storage, ushort dpi_x, ushort dpi_y)
{
    razer_report report = get_razer_report(0x04, 0x05, 0x07);
    report.arguments[0] = static_cast<uchar>(variable_storage);
    report.arguments[1] = (dpi_x >> 8) & 0x00FF;
    report.arguments[2] = dpi_x & 0x00FF;
    report.arguments[3] = (dpi_y >> 8) & 0x00FF;
    report.arguments[4] = dpi_y & 0x00FF;

    return report;
}

razer_report razer_chroma_misc_get_dpi_xy(RazerVarstore variable_storage)
{
    razer_report report = get_razer_report(0x04, 0x85, 0x07);
    report.arguments[0] = static_cast<uchar>(variable_storage);

    return report;
}

razer_report razer_chroma_misc_set_dpi_xy_byte(uchar dpi_x, uchar dpi_y)
{
    razer_report report = get_razer_report(0x04, 0x01, 0x03);

    report.arguments[0] = dpi_x;
    report.arguments[1] = dpi_y;
    report.arguments[2] = 0x00;

    return report;
}

razer_report razer_chroma_misc_get_dpi_xy_byte()
{
    razer_report report = get_razer_report(0x04, 0x81, 0x03);

    return report;
}

razer_report razer_chroma_misc_get_polling_rate()
{
    return get_razer_report(0x00, 0x85, 0x01);
}

razer_report razer_chroma_misc_set_polling_rate(uchar poll_rate_byte)
{
    razer_report report = get_razer_report(0x00, 0x05, 0x01);
    report.arguments[0] = poll_rate_byte;

    return report;
}
