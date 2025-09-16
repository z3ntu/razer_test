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

#include "razerled.h"

RazerLED::RazerLED(RazerDevice *device, openrazer::LedId ledId)
    : device(device), ledId(ledId)
{
}

RazerLED::~RazerLED() = default;

QDBusObjectPath RazerLED::getObjectPath()
{
    return QDBusObjectPath(QString("/io/github/openrazer1/devices/%1/led/%2").arg(device->getSerial()).arg(static_cast<uchar>(ledId)));
}

uchar RazerLED::getBrightness()
{
    // Wrapper as D-Bus can't (easily) handle pointers / multiple return values
    // TODO: Note: Apparently it can, I don't know how to properly implement it though.
    uchar brightness = 0x00;
    getBrightness(&brightness);
    return brightness;
}

openrazer::Effect RazerLED::getCurrentEffect()
{
    return effect;
}

QVector<openrazer::RGB> RazerLED::getCurrentColors()
{
    return { color1, color2, color3 };
}

openrazer::LedId RazerLED::getLedId()
{
    return ledId;
}

bool RazerLED::checkFx(QString fxStr)
{
    if (!device->checkFx(fxStr)) {
        dbusNotSupportedHelper(QString("%1: Unsupported FX: %2").arg(device->getName(), fxStr));
        return false;
    }
    return true;
}

bool RazerLED::hasFx(QString fxStr)
{
    return device->hasFx(fxStr);
}

void RazerLED::saveFxAndColors(openrazer::Effect fx, int numColors, openrazer::RGB color1, openrazer::RGB color2, openrazer::RGB color3)
{
    this->effect = fx;
    if (numColors >= 1) {
        this->color1 = color1;
    }
    if (numColors >= 2) {
        this->color2 = color2;
    }
    if (numColors >= 3) {
        this->color3 = color3;
    }
}

bool RazerLED::sendReportDBusHelper(razer_report request_report, razer_report *response_report)
{
    QString errorMessage;
    if (!device->sendReport(request_report, response_report, errorMessage)) {
        qCritical("%s", qUtf8Printable(errorMessage));
        if (calledFromDBus())
            sendErrorReply(QDBusError::Failed, qUtf8Printable(errorMessage));
        return false;
    }
    return true;
}

void RazerLED::dbusFailedHelper(const QString &errorMessage)
{
    qWarning("%s", qUtf8Printable(errorMessage));
    if (calledFromDBus())
        sendErrorReply(QDBusError::Failed, qUtf8Printable(errorMessage));
}

void RazerLED::dbusNotSupportedHelper(const QString &errorMessage)
{
    qWarning("%s", qUtf8Printable(errorMessage));
    if (calledFromDBus())
        sendErrorReply(QDBusError::NotSupported, qUtf8Printable(errorMessage));
}
