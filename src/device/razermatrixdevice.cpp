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

#include "razermatrixdevice.h"

bool RazerMatrixDevice::initializeLeds()
{
    foreach(RazerLedId ledId, ledIds) {
        RazerMatrixLED *rled = new RazerMatrixLED(ledId);
        bool ok;
        uchar brightness;
        ok = getBrightness(ledId, &brightness);
        if(!ok) {
            qDebug() << "Error during getBrightness()";
            return false;
        }
        if(!setSpectrumInit(ledId)) {
            qDebug() << "Error during setSpectrumInit()";
            return false;
        }
        rled->brightness = brightness;
        rled->effect = RazerMatrixEffectId::Spectrum;
        leds.insert(ledId, rled);
    }
    return true;
}

bool RazerMatrixDevice::setNone(RazerLedId led)
{
    qDebug() << "setNone() not implemented.";
    return false;
}

bool RazerMatrixDevice::setStatic(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug() << "setStatic() not implemented.";
    return false;
}

bool RazerMatrixDevice::setBreathing(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug() << "setBreathing() not implemented.";
    return false;
}

bool RazerMatrixDevice::setBlinking(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug() << "setBlinking() not implemented.";
    return false;
}

bool RazerMatrixDevice::setSpectrum(RazerLedId led)
{
    razer_report report, response_report;

    report = razer_chroma_standard_matrix_effect(RazerVarstore::STORE, led, RazerMatrixEffectId::Spectrum);
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    // Save state into LED variable
    RazerMatrixLED *rled = dynamic_cast<RazerMatrixLED*>(leds[led]);
    if(rled == NULL) {
        qDebug() << "Error while casting RazerLED into RazerMatrixLED";
        return false;
    }
    rled->effect = RazerMatrixEffectId::Spectrum;

    return true;
}

bool RazerMatrixDevice::setWave(RazerLedId led)
{
    qDebug() << "setWave() not implemented.";
    return false;
}

bool RazerMatrixDevice::setBrightness(RazerLedId led, uchar brightness)
{
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_brightness(RazerVarstore::STORE, led, brightness);
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    // Save state into LED variable
    RazerMatrixLED *rled = dynamic_cast<RazerMatrixLED*>(leds[led]);
    if(rled == NULL) {
        qDebug() << "Error while casting RazerLED into RazerMatrixLED";
        return false;
    }
    rled->brightness = brightness;

    return true;
}

bool RazerMatrixDevice::getBrightness(RazerLedId led, uchar *brightness)
{
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_brightness(RazerVarstore::STORE, led);
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    *brightness = response_report.arguments[2];

    return true;
}

bool RazerMatrixDevice::setSpectrumInit(RazerLedId led)
{
    razer_report report, response_report;

    report = razer_chroma_standard_matrix_effect(RazerVarstore::STORE, led, RazerMatrixEffectId::Spectrum);
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    return true;
}
