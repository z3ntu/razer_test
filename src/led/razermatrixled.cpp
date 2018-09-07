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

bool RazerMatrixLED::setNone(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if (!checkLedAndFx(led, "off"))
        return false;
    if (quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Off);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Off);
    }
}

bool RazerMatrixLED::setStatic(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue);
    if (!checkLedAndFx(led, "static"))
        return false;
    if (quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Static, 0x00, 0x00, 0x01, red, green, blue);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Static, red, green, blue);
    }
}

bool RazerMatrixLED::setBreathing(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue);
    if (!checkLedAndFx(led, "breathing"))
        return false;
    if (quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Breathing, 0x01, 0x00, 0x01, red, green, blue);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Breathing, 0x01, red, green, blue);
    }
}

bool RazerMatrixLED::setBreathingDual(RazerLedId led, uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2)
{
    qDebug("Called %s with params %hhu, %i, %i, %i, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue, red2, green2, blue2);
    if (!checkLedAndFx(led, "breathing_dual"))
        return false;
    if (quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Breathing, 0x02, 0x00, 0x02, red, green, blue, red2, green2, blue2);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Breathing, 0x02, red, green, blue, red2, green2, blue2);
    }
}

bool RazerMatrixLED::setBreathingRandom(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if (!checkLedAndFx(led, "breathing_random"))
        return false;
    if (quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Breathing);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Breathing, 0x03);
    }
}

bool RazerMatrixLED::setBlinking(RazerLedId led, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), red, green, blue);
    if (!checkLedAndFx(led, "blinking"))
        return false;
    if (calledFromDBus())
        sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerMatrixLED::setSpectrum(RazerLedId led)
{
    qDebug("Called %s with param %hhu", Q_FUNC_INFO, static_cast<uchar>(led));
    if (!checkLedAndFx(led, "spectrum"))
        return false;
    if (quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Spectrum);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Spectrum);
    }
}

bool RazerMatrixLED::setWave(RazerLedId led, WaveDirection direction)
{
    qDebug("Called %s with params %hhu, %hhu", Q_FUNC_INFO, static_cast<uchar>(led), static_cast<uchar>(direction));
    if (!checkLedAndFx(led, "wave"))
        return false;
    if (quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        // Wave direction is 0x00 / 0x01 instead of 0x01 / 0x02 for mouse_matrix, so subtract one
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Wave, static_cast<uchar>(direction) - 0x01, 0x28);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Wave, static_cast<uchar>(direction));
    }
}

bool RazerMatrixLED::setReactive(RazerLedId led, ReactiveSpeed speed, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(led), static_cast<uchar>(speed), red, green, blue);
    if (!checkLedAndFx(led, "reactive"))
        return false;
    if (quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return setMouseMatrixEffect(led, RazerMouseMatrixEffectId::Reactive, 0x00, static_cast<uchar>(speed), 0x01, red, green, blue);
    } else {
        return setMatrixEffect(led, RazerMatrixEffectId::Reactive, static_cast<uchar>(speed), red, green, blue);
    }
}

