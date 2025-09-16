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

#include "razermatrixled.h"

bool RazerMatrixLED::initialize()
{
    bool ok;
    ok = getBrightness(&brightness);
    if (!ok) {
        qWarning("Error during getBrightness()");
        return false;
    }
    if (!setSpectrum()) {
        qWarning("Error during setSpectrumInit()");
        return false;
    }
    effect = openrazer::Effect::Spectrum;
    return true;
}

bool RazerMatrixLED::setOff()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("off"))
        return false;
    saveFxAndColors(openrazer::Effect::Off, 0);
    if (device->hasQuirk(RazerDeviceQuirks::ExtendedMatrix)) {
        return setExtendedMatrixEffect(RazerMatrixEffectId::Off);
    } else if (device->hasQuirk(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Off);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Off);
    }
}

bool RazerMatrixLED::setOn()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("on"))
        return false;
    saveFxAndColors(openrazer::Effect::On, 0);
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerMatrixLED::setStatic(openrazer::RGB color)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, color.r, color.g, color.b);
    if (!checkFx("static"))
        return false;
    saveFxAndColors(openrazer::Effect::Static, 1, color);
    if (device->hasQuirk(RazerDeviceQuirks::ExtendedMatrix)) {
        return setExtendedMatrixEffect(RazerMatrixEffectId::Static, 0x00, 0x00, 0x01, color.r, color.g, color.b);
    } else if (device->hasQuirk(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Static, 0x00, 0x00, 0x01, color.r, color.g, color.b);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Static, color.r, color.g, color.b);
    }
}

bool RazerMatrixLED::setBreathing(openrazer::RGB color)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, color.r, color.g, color.b);
    if (!checkFx("breathing"))
        return false;
    saveFxAndColors(openrazer::Effect::Breathing, 1, color);
    if (device->hasQuirk(RazerDeviceQuirks::ExtendedMatrix)) {
        return setExtendedMatrixEffect(RazerMatrixEffectId::Breathing, 0x01, 0x00, 0x01, color.r, color.g, color.b);
    } else if (device->hasQuirk(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Breathing, 0x01, 0x00, 0x01, color.r, color.g, color.b);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Breathing, 0x01, color.r, color.g, color.b);
    }
}

bool RazerMatrixLED::setBreathingDual(openrazer::RGB color, openrazer::RGB color2)
{
    qDebug("Called %s with params %i, %i, %i, %i, %i, %i", Q_FUNC_INFO, color.r, color.g, color.b, color2.r, color2.g, color2.b);
    if (!checkFx("breathing_dual"))
        return false;
    saveFxAndColors(openrazer::Effect::BreathingDual, 2, color, color2);
    if (device->hasQuirk(RazerDeviceQuirks::ExtendedMatrix)) {
        return setExtendedMatrixEffect(RazerMatrixEffectId::Breathing, 0x02, 0x00, 0x02, color.r, color.g, color.b, color2.r, color2.g, color2.b);
    } else if (device->hasQuirk(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Breathing, 0x02, 0x00, 0x02, color.r, color.g, color.b, color2.r, color2.g, color2.b);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Breathing, 0x02, color.r, color.g, color.b, color2.r, color2.g, color2.b);
    }
}

bool RazerMatrixLED::setBreathingRandom()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("breathing_random"))
        return false;
    saveFxAndColors(openrazer::Effect::BreathingRandom, 0);
    if (device->hasQuirk(RazerDeviceQuirks::ExtendedMatrix)) {
        return setExtendedMatrixEffect(RazerMatrixEffectId::Breathing);
    } else if (device->hasQuirk(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Breathing);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Breathing, 0x03);
    }
}

bool RazerMatrixLED::setBlinking(openrazer::RGB color)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, color.r, color.g, color.b);
    if (!checkFx("blinking"))
        return false;

    dbusNotSupportedHelper("RazerMatrixLED does not implement setBlinking. This should not happen!");
    return false;
}

bool RazerMatrixLED::setSpectrum()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("spectrum"))
        return false;
    saveFxAndColors(openrazer::Effect::Spectrum, 0);
    if (device->hasQuirk(RazerDeviceQuirks::ExtendedMatrix)) {
        return setExtendedMatrixEffect(RazerMatrixEffectId::Spectrum);
    } else if (device->hasQuirk(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Spectrum);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Spectrum);
    }
}

bool RazerMatrixLED::setWave(openrazer::WaveDirection direction)
{
    qDebug("Called %s with params %hhu", Q_FUNC_INFO, static_cast<uchar>(direction));
    if (!checkFx("wave"))
        return false;
    saveFxAndColors(openrazer::Effect::Wave, 0);
    if (device->hasQuirk(RazerDeviceQuirks::ExtendedMatrix)) {
        // Wave direction is 0x00 / 0x01 instead of 0x01 / 0x02 for extended_matrix, so subtract one
        return setExtendedMatrixEffect(RazerMatrixEffectId::Wave, static_cast<uchar>(direction) - 0x01, 0x28);
    } else if (device->hasQuirk(RazerDeviceQuirks::MouseMatrix)) {
        // TODO
        qWarning("Unknown to set wave with mouse matrix");
        return false;
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Wave, static_cast<uchar>(direction));
    }
}

bool RazerMatrixLED::setReactive(openrazer::ReactiveSpeed speed, openrazer::RGB color)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(speed), color.r, color.g, color.b);
    if (!checkFx("reactive"))
        return false;
    saveFxAndColors(openrazer::Effect::Reactive, 1, color);
    if (device->hasQuirk(RazerDeviceQuirks::ExtendedMatrix)) {
        return setExtendedMatrixEffect(RazerMatrixEffectId::Reactive, 0x00, static_cast<uchar>(speed), 0x01, color.r, color.g, color.b);
    } else if (device->hasQuirk(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(RazerMouseMatrixEffectId::Reactive, 0x00, static_cast<uchar>(speed), 0x01, color.r, color.g, color.b);
    } else {
        return setMatrixEffect(RazerMatrixEffectId::Reactive, static_cast<uchar>(speed), color.r, color.g, color.b);
    }
}

bool RazerMatrixLED::setBrightness(uchar brightness)
{
    qDebug("Called %s with params %i", Q_FUNC_INFO, brightness);
    if (!checkFx("brightness"))
        return false;
    razer_report report, response_report;

    if (device->hasQuirk(RazerDeviceQuirks::MatrixBrightness)) {
        report = razer_chroma_extended_matrix_set_brightness(RazerVarstore::STORE, this->ledId, brightness);
    } else {
        report = razer_chroma_standard_set_led_brightness(RazerVarstore::STORE, this->ledId, brightness);
    }
    if (!sendReportDBusHelper(report, &response_report))
        return false;

    // Save state into LED variable
    this->brightness = brightness;

    return true;
}

bool RazerMatrixLED::getBrightness(uchar *brightness)
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("brightness"))
        return false;
    razer_report report, response_report;

    if (device->hasQuirk(RazerDeviceQuirks::MatrixBrightness)) {
        report = razer_chroma_extended_matrix_get_brightness(RazerVarstore::STORE, this->ledId);
    } else {
        report = razer_chroma_standard_get_led_brightness(RazerVarstore::STORE, this->ledId);
    }
    if (!sendReportDBusHelper(report, &response_report))
        return false;

    *brightness = response_report.arguments[2];

    return true;
}

/* --------------------- PRIVATE METHODS --------------------- */

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

    if (!sendReportDBusHelper(report, &response_report))
        return false;

    return true;
}

bool RazerMatrixLED::setExtendedMatrixEffect(RazerMatrixEffectId effect, uchar arg3, uchar arg4, uchar arg5, uchar arg6, uchar arg7, uchar arg8, uchar arg9, uchar arg10, uchar arg11)
{
    razer_report report, response_report;

    // Custom Frame uses 0x00 LED ID
    openrazer::LedId ledId = effect == RazerMatrixEffectId::CustomFrame ? openrazer::LedId::Unspecified : this->ledId;

    report = razer_chroma_extended_matrix_effect(RazerVarstore::STORE, ledId, effect);
    report.arguments[3] = arg3;
    report.arguments[4] = arg4;
    report.arguments[5] = arg5;
    report.arguments[6] = arg6;
    report.arguments[7] = arg7;
    report.arguments[8] = arg8;
    report.arguments[9] = arg9;
    report.arguments[10] = arg10;
    report.arguments[11] = arg11;

    if (!sendReportDBusHelper(report, &response_report))
        return false;

    return true;
}

bool RazerMatrixLED::setMouseMatrixEffect(RazerMouseMatrixEffectId effect, uchar arg3, uchar arg4, uchar arg5, uchar arg6, uchar arg7, uchar arg8, uchar arg9, uchar arg10, uchar arg11)
{
    razer_report report, response_report;

    report = razer_chroma_extended_mouse_matrix_effect(RazerVarstore::STORE, this->ledId, effect);
    report.arguments[3] = arg3;
    report.arguments[4] = arg4;
    report.arguments[5] = arg5;
    report.arguments[6] = arg6;
    report.arguments[7] = arg7;
    report.arguments[8] = arg8;
    report.arguments[9] = arg9;
    report.arguments[10] = arg10;
    report.arguments[11] = arg11;

    if (!sendReportDBusHelper(report, &response_report))
        return false;

    return true;
}
