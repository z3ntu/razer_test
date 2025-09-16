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
#include <openrazer.h>

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
inline size_t qHash(RazerClassicEffectId key, size_t seed)
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
    Reactive = 0x02,
    Breathing = 0x03,
    Spectrum = 0x04,
    Static = 0x06,
};

enum class RazerDeviceQuirks {
    ExtendedMatrix,
    MouseMatrix,
    MatrixBrightness,
    FireflyCustomFrame,
    ByteDPI,
};
inline size_t qHash(RazerDeviceQuirks key, size_t seed)
{
    return ::qHash(static_cast<uchar>(key), seed);
}

const QHash<QString, RazerDeviceQuirks> StringToQuirks {
    { "extended_matrix", RazerDeviceQuirks::ExtendedMatrix },
    { "mouse_matrix", RazerDeviceQuirks::MouseMatrix },
    { "matrix_brightness", RazerDeviceQuirks::MatrixBrightness },
    { "firefly_custom_frame", RazerDeviceQuirks::FireflyCustomFrame },
    { "byte_dpi", RazerDeviceQuirks::ByteDPI },
};

const QHash<RazerDeviceQuirks, QString> QuirksToString {
    { RazerDeviceQuirks::ExtendedMatrix, "extended_matrix" },
    { RazerDeviceQuirks::MouseMatrix, "mouse_matrix" },
    { RazerDeviceQuirks::MatrixBrightness, "matrix_brightness" },
    { RazerDeviceQuirks::FireflyCustomFrame, "firefly_custom_frame" },
    { RazerDeviceQuirks::ByteDPI, "byte_dpi" },
};

const QHash<QString, openrazer::LedId> StringToLedId {
    { "scroll_wheel", openrazer::LedId::ScrollWheelLED },
    { "battery", openrazer::LedId::BatteryLED },
    { "logo", openrazer::LedId::LogoLED },
    { "backlight", openrazer::LedId::BacklightLED },
    { "macro_recording", openrazer::LedId::MacroRecordingLED },
    { "game_mode", openrazer::LedId::GameModeLED },
    { "keymap_red", openrazer::LedId::KeymapRedLED },
    { "keymap_green", openrazer::LedId::KeymapGreenLED },
    { "keymap_blue", openrazer::LedId::KeymapBlueLED },
    { "right_side", openrazer::LedId::RightSideLED },
    { "left_side", openrazer::LedId::LeftSideLED },
};

const QHash<openrazer::LedId, QString> LedIdToString {
    { openrazer::LedId::ScrollWheelLED, "scroll_wheel" },
    { openrazer::LedId::BatteryLED, "battery" },
    { openrazer::LedId::LogoLED, "logo" },
    { openrazer::LedId::BacklightLED, "backlight" },
    { openrazer::LedId::MacroRecordingLED, "macro_recording" },
    { openrazer::LedId::GameModeLED, "game_mode" },
    { openrazer::LedId::KeymapRedLED, "keymap_red" },
    { openrazer::LedId::KeymapGreenLED, "keymap_green" },
    { openrazer::LedId::KeymapBlueLED, "keymap_blue" },
    { openrazer::LedId::RightSideLED, "right_side" },
    { openrazer::LedId::LeftSideLED, "left_side" }
};

const QHash<QString, openrazer::Effect> StringToEffect {
    { "off", openrazer::Effect::Off },
    { "on", openrazer::Effect::On },
    { "static", openrazer::Effect::Static },
    { "blinking", openrazer::Effect::Blinking },
    { "breathing", openrazer::Effect::Breathing },
    { "breathing_dual", openrazer::Effect::BreathingDual },
    { "breathing_random", openrazer::Effect::BreathingRandom },
    { "spectrum", openrazer::Effect::Spectrum },
    { "wave", openrazer::Effect::Wave },
    { "reactive", openrazer::Effect::Reactive }
};

const QHash<openrazer::Effect, QString> EffectToString {
    { openrazer::Effect::Off, "off" },
    { openrazer::Effect::On, "on" },
    { openrazer::Effect::Static, "static" },
    { openrazer::Effect::Blinking, "blinking" },
    { openrazer::Effect::Breathing, "breathing" },
    { openrazer::Effect::BreathingDual, "breathing_dual" },
    { openrazer::Effect::BreathingRandom, "breathing_random" },
    { openrazer::Effect::Spectrum, "spectrum" },
    { openrazer::Effect::Wave, "wave" },
    { openrazer::Effect::Reactive, "reactive" }
};

const QHash<uchar, QString> keyboardLayoutIds {
    { 0x01, "US" },
    { 0x02, "Greek" },
    { 0x03, "German" },
    { 0x04, "French" },
    { 0x05, "Russian" },
    { 0x06, "UK" },
    { 0x07, "Nordic" },
    { 0x0A, "Turkish" },
    { 0x0C, "Japanese" },
    { 0x10, "Spanish" },
    { 0x11, "Italian" },
    { 0x12, "Portuguese" },
    { 0x81, "US-mac" }
};

}

#endif // RAZERTESTPRIVATE_H
