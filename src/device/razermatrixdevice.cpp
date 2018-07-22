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

bool RazerMatrixDevice::setSpectrum(RazerLedId led)
{
    qDebug() << "setSpectrum() not implemented.";
    return false;
}

bool RazerMatrixDevice::setWave(RazerLedId led)
{
    qDebug() << "setWave() not implemented.";
    return false;
}

bool RazerMatrixDevice::setBrightness(RazerLedId led, uchar brightness)
{
    qDebug() << "setBrightness() not implemented.";
    return false;
}
