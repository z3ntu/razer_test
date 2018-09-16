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

#include "razermatrixdevice.h"

#include "../led/razermatrixled.h"

bool RazerMatrixDevice::initializeLeds()
{
    foreach (RazerLedId ledId, ledIds) {
        auto *rled = new RazerMatrixLED(this, ledId);
        bool ok;
        uchar brightness;
        ok = rled->getBrightness(&brightness);
        if (!ok) {
            qWarning("Error during getBrightness()");
            return false;
        }
        if (!rled->setSpectrumInit()) {
            qWarning("Error during setSpectrumInit()");
            return false;
        }
        rled->brightness = brightness;
        if (quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
            rled->mouseMatrixEffect = RazerMouseMatrixEffectId::Spectrum;
        } else {
            rled->effect = RazerMatrixEffectId::Spectrum;
        }
        leds.insert(ledId, rled);
    }
    return true;
}

/* --------------------- DBUS METHODS --------------------- */

bool RazerMatrixDevice::displayCustomFrame()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFx("custom_frame"))
        return false;

    RazerMatrixLED *led = static_cast<RazerMatrixLED *>(leds.values().first());
    if (quirks.contains(RazerDeviceQuirks::MouseMatrix)) {
        return led->setMouseMatrixEffect(RazerMouseMatrixEffectId::CustomFrame);
    } else {
        return led->setMatrixEffect(RazerMatrixEffectId::CustomFrame);
    }
}

bool RazerMatrixDevice::defineCustomFrame(uchar row, uchar startColumn, uchar endColumn, QByteArray rgbData)
{
    qDebug("Called %s with param %i, %i, %i, %s", Q_FUNC_INFO, row, startColumn, endColumn, rgbData.toHex().constData());
    if (!checkFx("custom_frame"))
        return false;

    if (rgbData.size() != ((endColumn + 1 - startColumn) * 3)) {
        qWarning("defineCustomFrame called with invalid size of rgbData");
        if (calledFromDBus())
            sendErrorReply(QDBusError::Failed);
        return false;
    }

    razer_report report, response_report;

    if (quirks.contains(RazerDeviceQuirks::FireflyCustomFrame)) {
        report = razer_chroma_misc_one_row_set_custom_frame(startColumn, endColumn, reinterpret_cast<const uchar *>(rgbData.constData()));
    } else {
        report = razer_chroma_standard_matrix_set_custom_frame(row, startColumn, endColumn, reinterpret_cast<const uchar *>(rgbData.constData()));
    }
    if (sendReport(report, &response_report) != 0) {
        if (calledFromDBus())
            sendErrorReply(QDBusError::Failed);
        return false;
    }
    return true;
}
