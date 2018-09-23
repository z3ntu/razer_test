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

#include "razerfakeled.h"

bool RazerFakeLED::setNone()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("off"))
        return false;
    saveFxAndColors(RazerEffect::Off, 0);
    return true;
}

bool RazerFakeLED::setStatic(uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, red, green, blue);
    if (!checkFx("static"))
        return false;
    saveFxAndColors(RazerEffect::Static, 1, {red, green, blue});
    return true;
}

bool RazerFakeLED::setBreathing(uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, red, green, blue);
    if (!checkFx("breathing"))
        return false;
    saveFxAndColors(RazerEffect::Breathing, 1, {red, green, blue});
    return true;
}

bool RazerFakeLED::setBreathingDual(uchar red, uchar green, uchar blue, uchar red2, uchar green2, uchar blue2)
{
    qDebug("Called %s with params %i, %i, %i, %i, %i, %i", Q_FUNC_INFO, red, green, blue, red2, green2, blue2);
    if (!checkFx("breathing_dual"))
        return false;
    saveFxAndColors(RazerEffect::BreathingDual, 1, {red, green, blue}, {red2, green2, blue2});
    return true;
}

bool RazerFakeLED::setBreathingRandom()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("breathing_random"))
        return false;
    saveFxAndColors(RazerEffect::BreathingRandom, 0);
    return true;
}

bool RazerFakeLED::setBlinking(uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %i, %i, %i", Q_FUNC_INFO, red, green, blue);
    if (!checkFx("blinking"))
        return false;
    saveFxAndColors(RazerEffect::Blinking, 1, {red, green, blue});
    return true;
}

bool RazerFakeLED::setSpectrum()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("spectrum"))
        return false;
    saveFxAndColors(RazerEffect::Spectrum, 0);
    return true;
}

bool RazerFakeLED::setWave(WaveDirection direction)
{
    qDebug("Called %s with params %hhu", Q_FUNC_INFO, static_cast<uchar>(direction));
    if (!checkFx("wave"))
        return false;
    saveFxAndColors(RazerEffect::Wave, 0);
    return true;
}

bool RazerFakeLED::setReactive(ReactiveSpeed speed, uchar red, uchar green, uchar blue)
{
    qDebug("Called %s with params %hhu, %i, %i, %i", Q_FUNC_INFO, static_cast<uchar>(speed), red, green, blue);
    if (!checkFx("reactive"))
        return false;
    saveFxAndColors(RazerEffect::Reactive, 1, {red, green, blue});
    return true;
}

bool RazerFakeLED::setBrightness(uchar brightness)
{
    qDebug("Called %s with params %i", Q_FUNC_INFO, brightness);
    if (!checkFx("brightness"))
        return false;
    this->brightness = brightness;
    return true;
}

bool RazerFakeLED::getBrightness(uchar *brightness)
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("brightness"))
        return false;
    *brightness = this->brightness;
    return true;
}
