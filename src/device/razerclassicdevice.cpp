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

#include "razerclassicdevice.h"

#include "../led/razerclassicled.h"

bool RazerClassicDevice::initializeLeds()
{
    foreach (RazerLedId ledId, ledIds) {
        RazerClassicLED *rled = new RazerClassicLED(this, ledId);
        bool ok;
        uchar brightness;
        RazerClassicEffectId effect;
        RazerClassicLedState state;
        uchar red, green, blue;
        ok = rled->getBrightness(ledId, &brightness);
        if (!ok) {
            qWarning("Error during getBrightness()");
            return false;
        }
        ok = rled->getLedEffect(ledId, &effect);
        if (!ok) {
            qWarning("Error during getLedEffect()");
            return false;
        }
        ok = rled->getLedState(ledId, &state);
        if (!ok) {
            qWarning("Error during getLedState()");
            return false;
        }
        ok = rled->getLedRgb(ledId, &red, &green, &blue);
        if (!ok) {
            qWarning("Error during getLedRgb()");
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

/* --------------------- DBUS METHODS --------------------- */

bool RazerClassicDevice::displayCustomFrame()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("custom_frame"))
        return false;
    sendErrorReply(QDBusError::NotSupported);
    return false;
}

bool RazerClassicDevice::defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QByteArray rgbData)
{
    qDebug("Called %s with param %i, %i, %i, %s", Q_FUNC_INFO, row, startColumn, endColumn, rgbData.toHex().constData());
    if (!checkFx("custom_frame"))
        return false;
    sendErrorReply(QDBusError::NotSupported);
    return false;
}
