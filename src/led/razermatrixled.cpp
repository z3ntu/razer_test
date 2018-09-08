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

#include "razermatrixled.h"

bool RazerMatrixLED::setNone()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!device->checkFx("off"))
        return false;
    if (device->quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Off);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Off);
    }
}

bool RazerMatrixLED::setStatic(uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, red, green, blue);
    if (!device->checkFx("static"))
        return false;
    if (device->quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Static, 0x00, 0x00, 0x01, red, green, blue);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Static, red, green, blue);
    }
}

bool RazerMatrixLED::setBreathing(uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, red, green, blue);
    if (!device->checkFx("breathing"))
        return false;
    if (device->quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Breathing, 0x01, 0x00, 0x01, red, green, blue);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Breathing, 0x01, red, green, blue);
    }
}

bool RazerMatrixLED::setBreathingDual(uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2)
{
    qDebug("Called %s with params %i, %i, %i, %i, %i, %i", Q_FUNC_INFO, red, green, blue, red2, green2, blue2);
    if (!device->checkFx("breathing_dual"))
        return false;
    if (device->quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Breathing, 0x02, 0x00, 0x02, red, green, blue, red2, green2, blue2);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Breathing, 0x02, red, green, blue, red2, green2, blue2);
    }
}

bool RazerMatrixLED::setBreathingRandom()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!device->checkFx("breathing_random"))
        return false;
    if (device->quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Breathing);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Breathing, 0x03);
    }
}

bool RazerMatrixLED::setBlinking(uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, red, green, blue);
    if (!device->checkFx("blinking"))
        return false;
    if (calledFromDBus())
        sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerMatrixLED::setSpectrum()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!device->checkFx("spectrum"))
        return false;
    if (device->quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Spectrum);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Spectrum);
    }
}

bool RazerMatrixLED::setWave(WaveDirection direction)
{
    qDebug("Called %s with params %hhu", Q_FUNC_INFO, static_cast<uchar>(direction));
    if (!device->checkFx("wave"))
        return false;
    if (device->quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        // Wave direction is 0x00 / 0x01 instead of 0x01 / 0x02 for mouse_matrix, so subtract one
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Wave, static_cast<uchar>(direction) - 0x01, 0x28);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Wave, static_cast<uchar>(direction));
    }
}

bool RazerMatrixLED::setReactive(ReactiveSpeed speed, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(speed), red, green, blue);
    if (!device->checkFx("reactive"))
        return false;
    if (device->quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Reactive, 0x00, static_cast<uchar>(speed), 0x01, red, green, blue);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Reactive, static_cast<uchar>(speed), red, green, blue);
    }
}

bool RazerMatrixLED::setBrightness(uchar brightness)
{
    qDebug("Called %s with params %i", Q_FUNC_INFO, brightness);
    if (!device->checkFx("brightness"))
        return false;
    razer_report report, response_report;

    if (device->quirks.contains(RazerDeviceQuirks::MatrixBrightness)) {
        report = razer_chroma_extended_matrix_set_brightness(RazerVarstore::STORE, this->ledId, brightness);
    } else {
        report = razer_chroma_standard_set_led_brightness(RazerVarstore::STORE, this->ledId, brightness);
    }
    if (device->sendReport(report, &response_report) != 0) {
        if (calledFromDBus())
            sendErrorReply(QDBusError::Failed);
        return false;
    }

    // Save state into LED variable
    this->brightness = brightness;

    return true;
}

bool RazerMatrixLED::getBrightness(uchar *brightness)
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!device->checkFx("brightness"))
        return false;
    razer_report report, response_report;

    if (device->quirks.contains(RazerDeviceQuirks::MatrixBrightness)) {
        report = razer_chroma_extended_matrix_get_brightness(RazerVarstore::STORE, this->ledId);
    } else {
        report = razer_chroma_standard_get_led_brightness(RazerVarstore::STORE, this->ledId);
    }
    if (device->sendReport(report, &response_report) != 0) {
        if (calledFromDBus())
            sendErrorReply(QDBusError::Failed);
        return false;
    }

    *brightness = response_report.arguments[2];

    return true;
}

/* --------------------- PRIVATE METHODS --------------------- */

bool RazerMatrixLED::setSpectrumInit()
{
    razer_report report, response_report;

    if (device->quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        report = razer_chroma_extended_mouse_matrix_effect(RazerVarstore::NOSTORE, this->ledId, RazerMouseMatrixEffectId::Spectrum);
    } else {
        report = razer_chroma_standard_matrix_effect(RazerMatrixEffectId::Spectrum);
    }
    if (device->sendReport(report, &response_report) != 0) {
        return false;
    }

    return true;
}

bool RazerMatrixLED::setMatrixEffect(RazerMatrixEffectId effect, uchar arg1, uchar arg2, uchar arg3, uchar arg4, uchar arg5, uchar arg6, uchar arg7, uchar arg8)
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

    if (device->sendReport(report, &response_report) != 0) {
        if (calledFromDBus())
            sendErrorReply(QDBusError::Failed);
        return false;
    }

    // Save state into LED variable
    this->effect = effect;

    return true;
}

bool RazerMatrixLED::setMouseMatrixEffect(RazerMouseMatrixEffectId effect, uchar arg3, uchar arg4, uchar arg5, uchar arg6, uchar arg7, uchar arg8, uchar arg9, uchar arg10, uchar arg11)
{
    razer_report report, response_report;

    report = razer_chroma_extended_mouse_matrix_effect(RazerVarstore::NOSTORE, this->ledId, effect);
    report.arguments[3] = arg3;
    report.arguments[4] = arg4;
    report.arguments[5] = arg5;
    report.arguments[6] = arg6;
    report.arguments[7] = arg7;
    report.arguments[8] = arg8;
    report.arguments[9] = arg9;
    report.arguments[10] = arg10;
    report.arguments[11] = arg11;

    if (device->sendReport(report, &response_report) != 0) {
        if (calledFromDBus())
            sendErrorReply(QDBusError::Failed);
        return false;
    }

    // Save state into LED variable
//     this->effect = effect; // FIXME

    return true;
}
