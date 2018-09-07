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
#include "../src/razerreport.h"

bool RazerClassicLED::setNone()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!device->checkFx("off"))
        return false;
    return setLedState(RazerClassicLedState::Off);
}

bool RazerClassicLED::setStatic(uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, red, green, blue);
    if (!device->checkFx("static"))
        return false;

    if (!ensureLedStateOn())
        return false;

    if (!setLedRgb(red, green, blue))
        return false;

    if (!setLedEffect(RazerClassicEffectId::Static))
        return false;

    return true;
}

bool RazerClassicLED::setBreathing(uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, red, green, blue);
    if (!device->checkFx("breathing"))
        return false;

    if (!ensureLedStateOn())
        return false;

    if (!setLedRgb(red, green, blue))
        return false;

    if (!setLedEffect(RazerClassicEffectId::Breathing))
        return false;

    return true;
}

bool RazerClassicLED::setBreathingDual(uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2)
{
    qDebug("Called %s with params %i, %i, %i, %i, %i, %i", Q_FUNC_INFO, red, green, blue, red2, green2, blue2);
    if (!device->checkFx("breathing_dual"))
        return false;
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicLED::setBreathingRandom()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!device->checkFx("breathing_random"))
        return false;
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicLED::setBlinking(uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, red, green, blue);
    if (!device->checkFx("blinking"))
        return false;

    if (!ensureLedStateOn())
        return false;

    if (!setLedRgb(red, green, blue))
        return false;

    if (!setLedEffect(RazerClassicEffectId::Blinking))
        return false;

    return true;
}

bool RazerClassicLED::setSpectrum()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!device->checkFx("spectrum"))
        return false;

    if (!ensureLedStateOn())
        return false;

    if (!setLedEffect(RazerClassicEffectId::Spectrum))
        return false;

    return true;
}

bool RazerClassicLED::setWave(WaveDirection direction)
{
    qDebug("Called %s with params %hhu", Q_FUNC_INFO, static_cast<uchar>(direction));
    if (!device->checkFx("wave"))
        return false;
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicLED::setReactive(ReactiveSpeed speed, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(speed), red, green, blue);
    if (!device->checkFx("reactive"))
        return false;
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicLED::setBrightness(uchar brightness)
{
    qDebug("Called %s with params %i", Q_FUNC_INFO, brightness);
    if (!device->checkFx(QString::null))
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
    if (!device->checkFx(QString::null)) // TODO ???
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
    this->state = state;

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
    if (stateChar < RazerClassicLedState::Off || stateChar > RazerClassicLedState::On) {
        qWarning("getLedState value is out of bounds!");
        return false;
    }
    *state = static_cast<RazerClassicLedState>(stateChar);

    return true;
}

bool RazerClassicLED::ensureLedStateOn()
{
    if (this->state == RazerClassicLedState::Off) {
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

    // Save state into LED variable
    this->effect = effect;

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
    if (effectChar < RazerClassicEffectId::Static || effectChar > RazerClassicEffectId::Spectrum) {
        qWarning("getLedEffect value is out of bounds!");
        return false;
    }
    *effect = static_cast<RazerClassicEffectId>(effectChar);

    return true;
}

bool RazerClassicLED::setLedRgb(uchar red, uchar green, uchar blue)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_rgb(RazerVarstore::STORE, this->ledId, red, green, blue);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    // Save state into LED variable
    this->red = red;
    this->green = green;
    this->blue = blue;

    return true;
}

bool RazerClassicLED::getLedRgb(uchar *red, uchar *green, uchar *blue)
{
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_rgb(RazerVarstore::STORE, this->ledId);
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    *red = response_report.arguments[2];
    *green = response_report.arguments[3];
    *blue = response_report.arguments[4];

    return true;
}
