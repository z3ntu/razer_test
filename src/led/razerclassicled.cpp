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

#include "razerclassicled.h"
#include "../razerreport.h"

bool RazerClassicLED::initialize()
{
    bool ok;
    RazerClassicEffectId classicEffect;
    ok = getBrightness(&brightness);
    if (!ok) {
        qWarning("Error during getBrightness()");
        return false;
    }
    ok = getLedEffect(&classicEffect);
    if (!ok) {
        qWarning("Error during getLedEffect()");
        return false;
    }
    ok = getLedState(&classicState);
    if (!ok) {
        qWarning("Error during getLedState()");
        return false;
    }
    // Translate effect to "RazerEffect effect"
    if (classicState == RazerClassicLedState::Off) {
        // State being Off is equivalent to Off effect
        effect = RazerEffect::Off;
    } else {
        effect = effectTranslationTable.value(classicEffect, RazerEffect::Off);
    }
    ok = getLedRgb(&color1);
    if (!ok) {
        qWarning("Error during getLedRgb()");
        return false;
    }
    return true;
}

bool RazerClassicLED::setOff()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("off"))
        return false;
    saveFxAndColors(RazerEffect::Off, 0);
    return setLedState(RazerClassicLedState::Off);
}

bool RazerClassicLED::setStatic(RGB color)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, color.r, color.g, color.b);
    if (!checkFx("static"))
        return false;
    saveFxAndColors(RazerEffect::Static, 1, color);

    if (!ensureLedStateOn())
        return false;

    if (!setLedRgb(color))
        return false;

    if (!setLedEffect(RazerClassicEffectId::Static))
        return false;

    return true;
}

bool RazerClassicLED::setBreathing(RGB color)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, color.r, color.g, color.b);
    if (!checkFx("breathing"))
        return false;
    saveFxAndColors(RazerEffect::Breathing, 1, color);

    if (!ensureLedStateOn())
        return false;

    if (!setLedRgb(color))
        return false;

    if (!setLedEffect(RazerClassicEffectId::Breathing))
        return false;

    return true;
}

bool RazerClassicLED::setBreathingDual(RGB color, RGB color2)
{
    qDebug("Called %s with params %i, %i, %i, %i, %i, %i", Q_FUNC_INFO, color.r, color.g, color.b, color2.r, color2.g, color2.b);
    if (!checkFx("breathing_dual"))
        return false;
    saveFxAndColors(RazerEffect::BreathingDual, 2, color, color2);
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicLED::setBreathingRandom()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("breathing_random"))
        return false;
    saveFxAndColors(RazerEffect::BreathingRandom, 0);
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicLED::setBlinking(RGB color)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, color.r, color.g, color.b);
    if (!checkFx("blinking"))
        return false;
    saveFxAndColors(RazerEffect::Blinking, 1, color);

    if (!ensureLedStateOn())
        return false;

    if (!setLedRgb(color))
        return false;

    if (!setLedEffect(RazerClassicEffectId::Blinking))
        return false;

    return true;
}

bool RazerClassicLED::setSpectrum()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("spectrum"))
        return false;
    saveFxAndColors(RazerEffect::Spectrum, 0);

    if (!ensureLedStateOn())
        return false;

    if (!setLedEffect(RazerClassicEffectId::Spectrum))
        return false;

    return true;
}

bool RazerClassicLED::setWave(WaveDirection direction)
{
    qDebug("Called %s with params %hhu", Q_FUNC_INFO, static_cast<uchar>(direction));
    if (!checkFx("wave"))
        return false;
    saveFxAndColors(RazerEffect::Wave, 0);
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicLED::setReactive(ReactiveSpeed speed, RGB color)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(speed), color.r, color.g, color.b);
    if (!checkFx("reactive"))
        return false;
    saveFxAndColors(RazerEffect::Reactive, 1, color);
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicLED::setBrightness(uchar brightness)
{
    qDebug("Called %s with params %i", Q_FUNC_INFO, brightness);
    if (!checkFx("brightness"))
        return false;
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_brightness(RazerVarstore::STORE, this->ledId, brightness);
    if (device->sendReport(report, &response_report) != 0) {
        sendErrorReply(QDBusError::Failed);
        return false;
    }

    // Save state into LED variable
    this->brightness = brightness;

    return true;
}

bool RazerClassicLED::getBrightness(uchar *brightness)
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("brightness"))
        return false;
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_brightness(RazerVarstore::STORE, this->ledId);
    if (device->sendReport(report, &response_report) != 0) {
        if (calledFromDBus())
            sendErrorReply(QDBusError::Failed);
        return false;
    }

    *brightness = response_report.arguments[2];

    return true;
}

/* --------------------- PRIVATE METHODS --------------------- */

bool RazerClassicLED::setLedState(RazerClassicLedState state)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_state(RazerVarstore::STORE, this->ledId, state);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    // Save state into LED variable
    this->classicState = state;

    return true;
}

bool RazerClassicLED::getLedState(RazerClassicLedState *state)
{
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_state(RazerVarstore::STORE, this->ledId);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    unsigned char stateChar = response_report.arguments[2];
//     if (stateChar < RazerClassicLedState::Off || stateChar > RazerClassicLedState::On) { // TODO: Needed?
//         qWarning("getLedState value is out of bounds!");
//         return false;
//     }
    *state = static_cast<RazerClassicLedState>(stateChar);

    return true;
}

bool RazerClassicLED::ensureLedStateOn()
{
    if (this->classicState == RazerClassicLedState::Off) {
        return setLedState(RazerClassicLedState::On);
    }
    return true;
}

bool RazerClassicLED::setLedEffect(RazerClassicEffectId effect)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_effect(RazerVarstore::STORE, this->ledId, effect);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    return true;
}

bool RazerClassicLED::getLedEffect(RazerClassicEffectId *effect)
{
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_effect(RazerVarstore::STORE, this->ledId);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    unsigned char effectChar = response_report.arguments[2];
//     if (effectChar < RazerClassicEffectId::Static || effectChar > RazerClassicEffectId::Spectrum) { // TODO: Needed?
//         qWarning("getLedEffect value is out of bounds!");
//         return false;
//     }
    *effect = static_cast<RazerClassicEffectId>(effectChar);

    return true;
}

bool RazerClassicLED::setLedRgb(RGB color)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_rgb(RazerVarstore::STORE, this->ledId, color.r, color.g, color.b);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    // State gets saved in methods calling this one

    return true;
}

bool RazerClassicLED::getLedRgb(RGB *color)
{
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_rgb(RazerVarstore::STORE, this->ledId);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    color->r = response_report.arguments[2];
    color->g = response_report.arguments[3];
    color->b = response_report.arguments[4];

    return true;
}
