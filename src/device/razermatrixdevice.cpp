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
        // TODO Create RazerMouseMatrixLED if(quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        RazerMatrixLED *rled = new RazerMatrixLED(ledId);
        bool ok;
        uchar brightness;
        ok = getBrightness(ledId, &brightness);
        if(!ok) {
            qWarning("Error during getBrightness()");
            return false;
        }
        if(!setSpectrumInit(ledId)) {
            qWarning("Error during setSpectrumInit()");
            return false;
        }
        rled->brightness = brightness;
        rled->effect = RazerMatrixEffectId::Spectrum;
        leds.insert(ledId, rled);
    }
    return true;
}

/* --------------------- DBUS METHODS --------------------- */

bool RazerMatrixDevice::setNone(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if(!checkLedAndFx(led, "off"))
        return false;
    if(quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Off);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Off);
    }
}

bool RazerMatrixDevice::setStatic(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue);
    if(!checkLedAndFx(led, "static"))
        return false;
    if(quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Static, 0x00, 0x00, 0x01, red, green, blue);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Static, red, green, blue);
    }
}

bool RazerMatrixDevice::setBreathing(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue);
    if(!checkLedAndFx(led, "breathing"))
        return false;
    if(quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Breathing, 0x01, 0x00, 0x01, red, green, blue);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Breathing, 0x01, red, green, blue);
    }
}

bool RazerMatrixDevice::setBreathingDual(RazerLedId led, uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2)
{
    qDebug("Called %s with params %hhu, %i, %i, %i, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue, red2, green2, blue2);
    if(!checkLedAndFx(led, "breathing_dual"))
        return false;
    if(quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Breathing, 0x02, 0x00, 0x02, red, green, blue, red2, green2, blue2);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Breathing, 0x02, red, green, blue, red2, green2, blue2);
    }
}

bool RazerMatrixDevice::setBreathingRandom(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if(!checkLedAndFx(led, "breathing_random"))
        return false;
    if(quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Breathing);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Breathing, 0x03);
    }
}

bool RazerMatrixDevice::setBlinking(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue);
    if(!checkLedAndFx(led, "blinking"))
        return false;
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerMatrixDevice::setSpectrum(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if(!checkLedAndFx(led, "spectrum"))
        return false;
    if(quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Spectrum);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Spectrum);
    }
}

bool RazerMatrixDevice::setWave(RazerLedId led, WaveDirection direction)
{
    qDebug("Called %s with params %hhu, %hhu", Q_FUNC_INFO, static_cast<uchar>(led), static_cast<uchar>(direction));
    if(!checkLedAndFx(led, "wave"))
        return false;
    if(quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        // Wave direction is 0x00 / 0x01 instead of 0x01 / 0x02 for mouse_matrix, so subtract one
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Wave, static_cast<uchar>(direction) - 0x01, 0x28);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Wave, static_cast<uchar>(direction));
    }
}

bool RazerMatrixDevice::setCustomFrame(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if(!checkLedAndFx(led, "custom_frame"))
        return false;
    if(quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::CustomFrame);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::CustomFrame);
    }
}

bool RazerMatrixDevice::setBrightness(RazerLedId led, uchar brightness)
{
    qDebug("Called %s with params %hhu, %i", Q_FUNC_INFO, static_cast<uchar>(led), brightness);
    if(!checkLedAndFx(led, QString::null))
        return false;
    razer_report report, response_report;

    report = razer_chroma_standard_set_led_brightness(RazerVarstore::STORE, led, brightness);
    if(sendReport(report, &response_report) != 0) {
        sendErrorReply(QDBusError::Failed);
        return false;
    }

    // Save state into LED variable
    RazerMatrixLED *rled = dynamic_cast<RazerMatrixLED*>(leds[led]);
    if(rled == NULL) {
        qWarning("Error while casting RazerLED into RazerMatrixLED");
        sendErrorReply(QDBusError::Failed);
        return false;
    }
    rled->brightness = brightness;

    return true;
}

bool RazerMatrixDevice::getBrightness(RazerLedId led, uchar *brightness)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if(!checkLedAndFx(led, QString::null))
        return false;
    razer_report report, response_report;

    report = razer_chroma_standard_get_led_brightness(RazerVarstore::STORE, led);
    if(sendReport(report, &response_report) != 0) {
        if(calledFromDBus())
            sendErrorReply(QDBusError::Failed);
        return false;
    }

    *brightness = response_report.arguments[2];

    return true;
}

/* --------------------- PRIVATE METHODS --------------------- */

bool RazerMatrixDevice::setSpectrumInit(RazerLedId led)
{
    razer_report report, response_report;

    if(quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        report = razer_chroma_extended_mouse_matrix_effect(RazerVarstore::NOSTORE, led, RazerMouseMatrixEffectId::Spectrum);
    } else {
        report = razer_chroma_standard_matrix_effect(RazerMatrixEffectId::Spectrum);
    }
    if(sendReport(report, &response_report) != 0) {
        return false;
    }

    return true;
}

bool RazerMatrixDevice::setMatrixEffect(RazerLedId led, RazerMatrixEffectId effect, uchar arg1, uchar arg2, uchar arg3, uchar arg4, uchar arg5, uchar arg6, uchar arg7, uchar arg8)
{
    razer_report report, response_report;

    report = razer_chroma_standard_matrix_effect(effect);
    report.arguments[1] = arg1;
    report.arguments[2] = arg2;
    report.arguments[3] = arg3;
    report.arguments[4] = arg4;
    report.arguments[5] = arg5;
    report.arguments[6] = arg6;
    report.arguments[7] = arg7;
    report.arguments[8] = arg8;

    if(sendReport(report, &response_report) != 0) {
        sendErrorReply(QDBusError::Failed);
        return false;
    }

    // Save state into LED variable
    RazerMatrixLED *rled = dynamic_cast<RazerMatrixLED*>(leds[led]);
    if(rled == NULL) {
        qWarning("Error while casting RazerLED into RazerMatrixLED");
        sendErrorReply(QDBusError::Failed);
        return false;
    }
    rled->effect = effect;

    return true;
}

bool RazerMatrixDevice::setMouseMatrixEffect(RazerLedId led, RazerMouseMatrixEffectId effect, uchar arg3, uchar arg4, uchar arg5, uchar arg6, uchar arg7, uchar arg8, uchar arg9, uchar arg10, uchar arg11)
{
    razer_report report, response_report;

    report = razer_chroma_extended_mouse_matrix_effect(RazerVarstore::NOSTORE, led, effect);
    report.arguments[3] = arg3;
    report.arguments[4] = arg4;
    report.arguments[5] = arg5;
    report.arguments[6] = arg6;
    report.arguments[7] = arg7;
    report.arguments[8] = arg8;
    report.arguments[9] = arg9;
    report.arguments[10] = arg10;
    report.arguments[11] = arg11;

    if(sendReport(report, &response_report) != 0) {
        sendErrorReply(QDBusError::Failed);
        return false;
    }

    // Save state into LED variable
    RazerMatrixLED *rled = dynamic_cast<RazerMatrixLED*>(leds[led]);
    if(rled == NULL) {
        qWarning("Error while casting RazerLED into RazerMatrixLED");
        sendErrorReply(QDBusError::Failed);
        return false;
    }
    // FIXME
//     rled->effect = effect;

    return true;
}
