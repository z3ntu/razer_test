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

#include <QDebug>

#include "razerclassicdevice.h"

bool RazerClassicDevice::initializeLeds()
{
    foreach(RazerLedId ledId, ledIds) {
        RazerClassicLED *rled = new RazerClassicLED(ledId);
        bool ok;
        uchar brightness;
        RazerClassicEffectId effect;
        RazerClassicLedState state;
        uchar red, green, blue;
        ok = getBrightness(ledId, &brightness);
        if(!ok) {
            qDebug() << "Error during getBrightness()";
            return false;
        }
        ok = getLedEffect(ledId, &effect);
        if(!ok) {
            qDebug() << "Error during getLedEffect()";
            return false;
        }
        ok = getLedState(ledId, &state);
        if(!ok) {
            qDebug() << "Error during getLedState()";
            return false;
        }
        ok = getLedRgb(ledId, &red, &green, &blue);
        if(!ok) {
            qDebug() << "Error during getLedRgb()";
            return false;
        }
        rled->brightness = brightness;
        rled->effect = effect;
        rled->state = state;
        rled->red = red;
        rled->green = green;
        rled->blue = blue;
        leds.insert(ledId, rled);
    }
    return true;
}

bool RazerClassicDevice::setNone(RazerLedId led)
{
    return setLedState(led, RazerClassicLedState::Off);
}

bool RazerClassicDevice::setStatic(RazerLedId led, uchar red, uchar green, uchar blue)
{
    ensureLedStateOn(led);

    if(!setLedRgb(led, red, green, blue))
        return false;

    if(!setLedEffect(led, RazerClassicEffectId::Static))
        return false;

    return true;
}

bool RazerClassicDevice::setBreathing(RazerLedId led, uchar red, uchar green, uchar blue)
{
    ensureLedStateOn(led);

    if(!setLedRgb(led, red, green, blue))
        return false;

    if(!setLedEffect(led, RazerClassicEffectId::Pulsating))
        return false;

    return true;
}

bool RazerClassicDevice::setBlinking(RazerLedId led, uchar red, uchar green, uchar blue)
{
    ensureLedStateOn(led);

    if(!setLedRgb(led, red, green, blue))
        return false;

    if(!setLedEffect(led, RazerClassicEffectId::Blinking))
        return false;

    return true;
}

bool RazerClassicDevice::setSpectrum(RazerLedId led)
{
    ensureLedStateOn(led);

    if(!setLedEffect(led, RazerClassicEffectId::Spectrum))
        return false;

    return true;
}

bool RazerClassicDevice::setWave(RazerLedId led)
{
    qDebug() << "setWave() not implemented.";
    return false;
}

bool RazerClassicDevice::setBrightness(RazerLedId led, uchar brightness)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_brightness(RazerVarstore::STORE, led, brightness);
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    // Save state into LED variable
    RazerClassicLED *rled = dynamic_cast<RazerClassicLED*>(leds[led]);
    if(rled == NULL) {
        qDebug() << "Error while casting RazerLED into RazerClassicLED";
        return false;
    }
    rled->brightness = brightness;

    return true;
}

bool RazerClassicDevice::getBrightness(RazerLedId led, uchar *brightness)
{
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_brightness(RazerVarstore::STORE, led);
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    *brightness = response_report.arguments[2];

    return true;
}

bool RazerClassicDevice::setLedState(RazerLedId led, RazerClassicLedState state)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_state(RazerVarstore::STORE, led, state);
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    // Save state into LED variable
    RazerClassicLED *rled = dynamic_cast<RazerClassicLED*>(leds[led]);
    if(rled == NULL) {
        qDebug() << "Error while casting RazerLED into RazerClassicLED";
        return false;
    }
    rled->state = state;

    return true;
}

bool RazerClassicDevice::getLedState(RazerLedId led, RazerClassicLedState *state)
{
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_state(RazerVarstore::STORE, led);
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    unsigned char stateChar = response_report.arguments[2];
    if(stateChar < RazerClassicLedState::Off || stateChar > RazerClassicLedState::On) {
        qDebug() << "getLedState value is out of bounds!";
        return false;
    }
    *state = static_cast<RazerClassicLedState>(stateChar);

    return true;
}

bool RazerClassicDevice::ensureLedStateOn(RazerLedId led)
{
    RazerClassicLED *rled = dynamic_cast<RazerClassicLED*>(leds[led]);
    if(rled == NULL) {
        qDebug() << "Error while casting RazerLED into RazerClassicLED";
        return false;
    }
    if(rled->state == RazerClassicLedState::Off) {
        return setLedState(led, RazerClassicLedState::On);
    }
    return true;
}

bool RazerClassicDevice::setLedEffect(RazerLedId led, RazerClassicEffectId effect)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_effect(RazerVarstore::STORE, led, effect);
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    // Save state into LED variable
    RazerClassicLED *rled = dynamic_cast<RazerClassicLED*>(leds[led]);
    if(rled == NULL) {
        qDebug() << "Error while casting RazerLED into RazerClassicLED";
        return false;
    }
    rled->effect = effect;

    return true;
}

bool RazerClassicDevice::getLedEffect(RazerLedId led, RazerClassicEffectId *effect)
{
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_effect(RazerVarstore::STORE, led);
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    unsigned char effectChar = response_report.arguments[2];
    if(effectChar < RazerClassicEffectId::Static || effectChar > RazerClassicEffectId::Spectrum) {
        qDebug() << "getLedEffect value is out of bounds!";
        return false;
    }
    *effect = static_cast<RazerClassicEffectId>(effectChar);

    return true;
}

bool RazerClassicDevice::setLedRgb(RazerLedId led, uchar red, uchar green, uchar blue)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_rgb(RazerVarstore::STORE, led, red, green, blue);
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    // Save state into LED variable
    RazerClassicLED *rled = dynamic_cast<RazerClassicLED*>(leds[led]);
    if(rled == NULL) {
        qDebug() << "Error while casting RazerLED into RazerClassicLED";
        return false;
    }
    rled->red = red;
    rled->green = green;
    rled->blue = blue;

    return true;
}

bool RazerClassicDevice::getLedRgb(RazerLedId led, uchar* red, uchar* green, uchar* blue)
{
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_rgb(RazerVarstore::STORE, led);
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    *red = response_report.arguments[2];
    *green = response_report.arguments[3];
    *blue = response_report.arguments[4];

    return true;
}
