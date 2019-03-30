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

#ifndef RAZERTESTPRIVATE_H
#define RAZERTESTPRIVATE_H

#include <QtGlobal>

namespace razer_test {

enum class RazerVarstore : uchar {
    NOSTORE = 0x00,
    STORE = 0x01
};

// TODO: Convert to enum class
enum RazerStatus {
    NEW = 0x00,
    BUSY = 0x01,
    SUCCESSFUL = 0x02,
    FAILURE = 0x03,
    NO_RESPONSE_TIMEOUT = 0x04,
    NOT_SUPPORTED = 0x05
};

enum class RazerClassicLedState : uchar {
    Off = 0x00,
    On = 0x01
};

enum class RazerClassicEffectId : uchar {
    Static = 0x00,
    Blinking = 0x01,
    Breathing = 0x02, // Pulsating
    Spectrum = 0x04
};
inline uint qHash(RazerClassicEffectId key, uint seed)
{
    return ::qHash(static_cast<uchar>(key), seed);
}

enum class RazerMatrixEffectId : uchar {
    Off = 0x00,
    Wave = 0x01,
    Reactive = 0x02, // Afterglow
    Breathing = 0x03,
    Spectrum = 0x04,
    CustomFrame = 0x05,
    Static = 0x06,
    Starlight = 0x19
};

enum class RazerMouseMatrixEffectId : uchar {
    Off = 0x00,
    Static = 0x01,
    Breathing = 0x02,
    Spectrum = 0x03,
    Wave = 0x04,
    Reactive = 0x05,
    Starlight = 0x07,
    CustomFrame = 0x08
};

enum class RazerDeviceQuirks {
    MouseMatrix,
    MatrixBrightness,
    NoGetBrightness,
    FireflyCustomFrame
};
inline uint qHash(RazerDeviceQuirks key, uint seed)
{
    return ::qHash(static_cast<uchar>(key), seed);
}

const QHash<QString, RazerDeviceQuirks> StringToQuirks {
    { "mouse_matrix", RazerDeviceQuirks::MouseMatrix },
    { "matrix_brightness", RazerDeviceQuirks::MatrixBrightness },
    { "no_get_brightness", RazerDeviceQuirks::NoGetBrightness },
    { "firefly_custom_frame", RazerDeviceQuirks::FireflyCustomFrame },
};

const QHash<RazerDeviceQuirks, QString> QuirksToString {
    { RazerDeviceQuirks::MouseMatrix, "mouse_matrix" },
    { RazerDeviceQuirks::MatrixBrightness, "matrix_brightness" },
    { RazerDeviceQuirks::NoGetBrightness, "no_get_brightness" },
    { RazerDeviceQuirks::FireflyCustomFrame, "firefly_custom_frame" },
};

const QHash<QString, RazerLedId> StringToLedId {
    { "scroll_wheel", RazerLedId::ScrollWheelLED },
    { "battery", RazerLedId::BatteryLED },
    { "logo", RazerLedId::LogoLED },
    { "backlight", RazerLedId::BacklightLED },
    { "macro_recording", RazerLedId::MacroRecordingLED },
    { "game_mode", RazerLedId::GameModeLED },
    { "keymap_red", RazerLedId::KeymapRedLED },
    { "keymap_green", RazerLedId::KeymapGreenLED },
    { "keymap_blue", RazerLedId::KeymapBlueLED },
    { "right_side", RazerLedId::RightSideLED },
    { "left_side", RazerLedId::LeftSideLED }
};

const QHash<RazerLedId, QString> LedIdToString {
    { RazerLedId::ScrollWheelLED, "scroll_wheel" },
    { RazerLedId::BatteryLED, "battery" },
    { RazerLedId::LogoLED, "logo" },
    { RazerLedId::BacklightLED, "backlight" },
    { RazerLedId::MacroRecordingLED, "macro_recording" },
    { RazerLedId::GameModeLED, "game_mode" },
    { RazerLedId::KeymapRedLED, "keymap_red" },
    { RazerLedId::KeymapGreenLED, "keymap_green" },
    { RazerLedId::KeymapBlueLED, "keymap_blue" },
    { RazerLedId::RightSideLED, "right_side" },
    { RazerLedId::LeftSideLED, "left_side" }
};

const QHash<QString, RazerEffect> StringToEffect {
    { "off", RazerEffect::Off },
    { "static", RazerEffect::Static },
    { "blinking", RazerEffect::Blinking },
    { "breathing", RazerEffect::Breathing },
    { "breathing_dual", RazerEffect::BreathingDual },
    { "breathing_random", RazerEffect::BreathingRandom },
    { "spectrum", RazerEffect::Spectrum },
    { "wave", RazerEffect::Wave },
    { "reactive", RazerEffect::Reactive }
};

const QHash<RazerEffect, QString> EffectToString {
    { RazerEffect::Off, "off" },
    { RazerEffect::Static, "static" },
    { RazerEffect::Blinking, "blinking" },
    { RazerEffect::Breathing, "breathing" },
    { RazerEffect::BreathingDual, "breathing_dual" },
    { RazerEffect::BreathingRandom, "breathing_random" },
    { RazerEffect::Spectrum, "spectrum" },
    { RazerEffect::Wave, "wave" },
    { RazerEffect::Reactive, "reactive" }
};

const QHash<uchar, QString> keyboardLayoutIds {
    { 0x01, "US" },
    { 0x02, "Greek" },
    { 0x03, "German" },
    { 0x04, "French" },
    { 0x06, "UK" },
    { 0x07, "Nordic" },
    { 0x10, "Spanish" },
    { 0x11, "Italian" },
    { 0x12, "Portuguese" },
    { 0x81, "US-mac" }
};

}

#endif // RAZERTESTPRIVATE_H
