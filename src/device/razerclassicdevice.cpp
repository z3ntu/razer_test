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

bool RazerClassicDevice::setNone(RazerLedId led)
{
    return setLedState(led, RazerClassicLedState::Off);
}

bool RazerClassicDevice::setStatic(RazerLedId led, uchar red, uchar green, uchar blue)
{
    ensureLedStateOn(led);

    razer_report report, response_report;

    report = razer_chroma_standard_set_led_rgb(RazerVarstore::STORE, led, red, green, blue);
    if(sendReport(report, &response_report) != 0)
        return false;

    // TODO: Save state into LED variable

    report = razer_chroma_standard_set_led_effect(RazerVarstore::STORE, led, RazerClassicEffectId::Static);
    if(sendReport(report, &response_report) != 0)
        return false;

    // TODO: Save state into LED variable

    return true;
}

bool RazerClassicDevice::setBreathing(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug() << "setBreathing() not implemented.";
    return false;
}

bool RazerClassicDevice::setSpectrum(RazerLedId led)
{
    qDebug() << "setSpectrum() not implemented.";
    return false;
}

bool RazerClassicDevice::setWave(RazerLedId led)
{
    qDebug() << "setWave() not implemented.";
    return false;
}

bool RazerClassicDevice::setBrightness(RazerLedId led, uchar brightness)
{
    qDebug() << "setBrightness() not implemented.";
    return false;
}

bool RazerClassicDevice::setLedState(RazerLedId led, RazerClassicLedState state)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_state(RazerVarstore::STORE, led, state);
    if(sendReport(report, &response_report) != 0) {
        // TODO: Save state into LED variable
        return false;
    }
    // TODO: Save state into LED variable

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
