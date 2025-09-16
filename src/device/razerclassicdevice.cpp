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

#include "razerclassicdevice.h"

#include "../led/razerclassicled.h"

bool RazerClassicDevice::initialize()
{
    foreach (openrazer::LedId ledId, ledIds) {
        auto *rled = new RazerClassicLED(this, ledId);
        if (!rled->initialize()) {
            qWarning("Error while initializing LED with ID '%hhu'", static_cast<uchar>(ledId));
            delete rled;
            return false;
        }
        leds.insert(ledId, rled);
    }
    return true;
}

/* --------------------- DBUS METHODS --------------------- */

bool RazerClassicDevice::displayCustomFrame()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("custom_frame"))
        return false;

    dbusNotSupportedHelper("RazerClassicDevice does not implement displayCustomFrame. This should not happen!");
    return false;
}

bool RazerClassicDevice::defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QVector<openrazer::RGB> rgbData)
{
    qDebug("Called %s with param %i, %i, %i", Q_FUNC_INFO, row, startColumn, endColumn);
    qDebug() << " (cont.) rgbData:" << rgbData;
    if (!checkFeature("custom_frame"))
        return false;

    dbusNotSupportedHelper("RazerClassicDevice does not implement defineCustomFrame. This should not happen!");
    return false;
}
