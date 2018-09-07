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
#include <../src/razerreport.h>

bool RazerClassicLED::setNone(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if (!checkLedAndFx(led, "off"))
        return false;
    return setLedState(led, RazerClassicLedState::Off);
}

bool RazerClassicLED::setStatic(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue);
    if (!checkLedAndFx(led, "static"))
        return false;

    if (!ensureLedStateOn(led))
        return false;

    if (!setLedRgb(led, red, green, blue))
        return false;

    if (!setLedEffect(led, RazerClassicEffectId::Static))
        return false;

    return true;
}

bool RazerClassicLED::setBreathing(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue);
    if (!checkLedAndFx(led, "breathing"))
        return false;

    if (!ensureLedStateOn(led))
        return false;

    if (!setLedRgb(led, red, green, blue))
        return false;

    if (!setLedEffect(led, RazerClassicEffectId::Breathing))
        return false;

    return true;
}

bool RazerClassicLED::setBreathingDual(RazerLedId led, uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2)
{
    qDebug("Called %s with params %hhu, %i, %i, %i, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue, red2, green2, blue2);
    if (!checkLedAndFx(led, "breathing_dual"))
        return false;
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicLED::setBreathingRandom(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if (!checkLedAndFx(led, "breathing_random"))
        return false;
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicLED::setBlinking(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue);
    if (!checkLedAndFx(led, "blinking"))
        return false;

    if (!ensureLedStateOn(led))
        return false;

    if (!setLedRgb(led, red, green, blue))
        return false;

    if (!setLedEffect(led, RazerClassicEffectId::Blinking))
        return false;

    return true;
}

bool RazerClassicLED::setSpectrum(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if (!checkLedAndFx(led, "spectrum"))
        return false;

    if (!ensureLedStateOn(led))
        return false;

    if (!setLedEffect(led, RazerClassicEffectId::Spectrum))
        return false;

    return true;
}

bool RazerClassicLED::setWave(RazerLedId led, WaveDirection direction)
{
    qDebug("Called %s with params %hhu, %hhu", Q_FUNC_INFO, static_cast<uchar>(led), static_cast<uchar>(direction));
    if (!checkLedAndFx(led, "wave"))
        return false;
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicLED::setReactive(RazerLedId led, ReactiveSpeed speed, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), static_cast<uchar>(speed), red, green, blue);
    if (!checkLedAndFx(led, "reactive"))
        return false;
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicLED::setBrightness(RazerLedId led, uchar brightness)
{
    qDebug("Called %s with params %hhu, %i", Q_FUNC_INFO, static_cast<uchar>(led), brightness);
    if (!checkLedAndFx(led, QString::null))
        return false;
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_brightness(RazerVarstore::STORE, led, brightness);
    if (this->sendReport(report, &response_report) != 0) {
        sendErrorReply(QDBusError::Failed);
        return false;
    }

    // Save state into LED variable
    RazerClassicLED *rled = dynamic_cast<RazerClassicLED *>(leds[led]);
    if (rled == NULL) {
        qWarning("Error while casting RazerLED into RazerClassicLED");
        sendErrorReply(QDBusError::Failed);
        return false;
    }
    rled->brightness = brightness;

    return true;
}

bool RazerClassicLED::getBrightness(RazerLedId led, uchar *brightness)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if (!checkLedAndFx(led, QString::null))
        return false;
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_brightness(RazerVarstore::STORE, led);
    if (this->sendReport(report, &response_report) != 0) {
        if (calledFromDBus())
            sendErrorReply(QDBusError::Failed);
        return false;
    }

    *brightness = response_report.arguments[2];

    return true;
}

/* --------------------- PRIVATE METHODS --------------------- */

bool RazerClassicLED::setLedState(RazerLedId led, RazerClassicLedState state)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_state(RazerVarstore::STORE, led, state);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    // Save state into LED variable
    RazerClassicLED *rled = dynamic_cast<RazerClassicLED *>(leds[led]);
    if (rled == NULL) {
        qWarning("Error while casting RazerLED into RazerClassicLED");
        return false;
    }
    rled->state = state;

    return true;
}

bool RazerClassicLED::getLedState(RazerLedId led, RazerClassicLedState *state)
{
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_state(RazerVarstore::STORE, led);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    unsigned char stateChar = response_report.arguments[2];
    if (stateChar < RazerClassicLedState::Off || stateChar > RazerClassicLedState::On) {
        qWarning("getLedState value is out of bounds!");
        return false;
    }
    *state = static_cast<RazerClassicLedState>(stateChar);

    return true;
}

bool RazerClassicLED::ensureLedStateOn(RazerLedId led)
{
    RazerClassicLED *rled = dynamic_cast<RazerClassicLED *>(leds[led]);
    if (rled == NULL) {
        qWarning("Error while casting RazerLED into RazerClassicLED");
        return false;
    }
    if (rled->state == RazerClassicLedState::Off) {
        return setLedState(led, RazerClassicLedState::On);
    }
    return true;
}

bool RazerClassicLED::setLedEffect(RazerLedId led, RazerClassicEffectId effect)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_effect(RazerVarstore::STORE, led, effect);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    // Save state into LED variable
    RazerClassicLED *rled = dynamic_cast<RazerClassicLED *>(leds[led]);
    if (rled == NULL) {
        qWarning("Error while casting RazerLED into RazerClassicLED");
        return false;
    }
    rled->effect = effect;

    return true;
}

bool RazerClassicLED::getLedEffect(RazerLedId led, RazerClassicEffectId *effect)
{
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_effect(RazerVarstore::STORE, led);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    unsigned char effectChar = response_report.arguments[2];
    if (effectChar < RazerClassicEffectId::Static || effectChar > RazerClassicEffectId::Spectrum) {
        qWarning("getLedEffect value is out of bounds!");
        return false;
    }
    *effect = static_cast<RazerClassicEffectId>(effectChar);

    return true;
}

bool RazerClassicLED::setLedRgb(RazerLedId led, uchar red, uchar green, uchar blue)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_rgb(RazerVarstore::STORE, led, red, green, blue);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    // Save state into LED variable
    RazerClassicLED *rled = dynamic_cast<RazerClassicLED *>(leds[led]);
    if (rled == NULL) {
        qWarning("Error while casting RazerLED into RazerClassicLED");
        return false;
    }
    rled->red = red;
    rled->green = green;
    rled->blue = blue;

    return true;
}

bool RazerClassicLED::getLedRgb(RazerLedId led, uchar *red, uchar *green, uchar *blue)
{
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_rgb(RazerVarstore::STORE, led);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    *red = response_report.arguments[2];
    *green = response_report.arguments[3];
    *blue = response_report.arguments[4];

    return true;
}
