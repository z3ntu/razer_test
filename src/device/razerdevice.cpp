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

#include <cstdio>
#include <cstring>

#include <QThread>

#include "razerdevice.h"

RazerDevice::RazerDevice(QString dev_path, ushort vendor_id, ushort product_id, QString name, QString type, QVector<RazerLedId> ledIds, QStringList fx, QStringList features, QVector<RazerDeviceQuirks> quirks, MatrixDimensions matrixDimensions, ushort maxDPI)
{
    this->dev_path = dev_path;
    this->vendor_id = vendor_id;
    this->product_id = product_id;
    this->name = name;
    this->type = type;
    this->ledIds = ledIds;
    this->fx = fx;
    this->features = features;
    this->quirks = quirks;
    this->matrixDimensions = matrixDimensions;
    this->maxDPI = maxDPI;

    this->thread = new CustomEffectThread(matrixDimensions.x, matrixDimensions.y);

    // Connect CustomEffectThread with the device
    connect(thread, &CustomEffectThread::rgbDataReady, this, &RazerDevice::customRgbDataReady);
    connect(thread, &CustomEffectThread::frameReady, this, &RazerDevice::customFrameReady);
}

RazerDevice::~RazerDevice()
{
    // Destroy LEDs
    foreach (RazerLED *led, leds) {
        delete led;
    }
    // Destroy CustomEffectThread
    delete thread;
    // Close hidapi handle
    if (handle != nullptr)
        hid_close(handle);
}

bool RazerDevice::openDeviceHandle()
{
    if (dev_path == nullptr) {
        qCritical("dev_path is NULL but openDeviceHandle() was called. This should not happen!");
        return false;
    }
    handle = hid_open_path(dev_path.toStdString().c_str());
    if (!handle) {
        return false;
    }
    return true;
}

bool RazerDevice::sendReport(razer_report request_report, razer_report *response_report, QString &errorMessage)
{
    if (handle == nullptr) {
        errorMessage = "sendReport called on an unopened handle. This should not happen!";
        return false;
    }
    int res;
    unsigned char req_buf[sizeof(razer_report) + 1];
    unsigned char res_buf[sizeof(razer_report) + 1];

    // Calculate the crc
    request_report.crc = razer_calculate_crc(&request_report);

    // Copy request_report into req_buf, shifted by 1 byte to the right for the report number
    req_buf[0] = 0x00; // report number
    memcpy(&req_buf[1], &request_report, sizeof(razer_report));

#ifdef DEBUG
    printf("Request report: ");
    for (int i = 1; i < sizeof(razer_report) + 1; i++)
        printf("%02hhx ", req_buf[i]);
    printf("\n");
#endif

    int retryCount = 3;

    while (retryCount > 0) {

        // Send the Feature Report to the device
        res = hid_send_feature_report(handle, req_buf, sizeof(req_buf));
        if (res < 0) {
            qWarning("Failed to send a feature report, retrying...");
            retryCount--;
            continue;
        }

        // Wait a bit
        if (retryCount == 3)
            QThread::usleep(800);
        else
            QThread::usleep(8000); // let's sleep a bit longer, still only 0.008 seconds

        // Read a Feature Report from the device
        res_buf[0] = 0x00; // report number
        res = hid_get_feature_report(handle, res_buf, sizeof(res_buf));
        if (res < 0) {
            qWarning("Failed to get a feature report, retrying...");
            retryCount--;
            continue;
        }

#ifdef DEBUG
        printf("Response report: ");
        for (int i = 1; i < sizeof(razer_report) + 1; i++)
            printf("%02hhx ", res_buf[i]);
        printf("\n");
#endif

        // Copy returned data into the response_report, minus the report number
        memcpy(response_report, &res_buf[1], sizeof(razer_report));

#ifdef DEBUG
        printf("Response report: Status: %02x transaction id: %02x Data size: %02x Command class: %02x Command id: %02x\n",
               response_report->status,
               response_report->transaction_id,
               response_report->data_size,
               response_report->command_class,
               response_report->command_id);
#endif

        if (response_report->status == RazerStatus::NOT_SUPPORTED) {
            errorMessage = "Failed to send report - device returned NOT_SUPPORTED";
            return false;
        }

        if (response_report->status != RazerStatus::SUCCESSFUL) {
            retryCount--;
            continue;
        } else {
            return true;
        }
    }

    errorMessage = "Failed to send report after 3 tries.";
    return false;
}

QDBusObjectPath RazerDevice::getObjectPath()
{
    return QDBusObjectPath(QString("/io/github/openrazer1/devices/%1").arg(getSerial()));
}

/* --------------------- DBUS METHODS --------------------- */

QString RazerDevice::getName()
{
    qDebug("Called %s", Q_FUNC_INFO);
    return name;
}

QString RazerDevice::getType()
{
    qDebug("Called %s", Q_FUNC_INFO);
    return type;
}

QStringList RazerDevice::getSupportedFx()
{
    qDebug("Called %s", Q_FUNC_INFO);
    return fx;
}

QStringList RazerDevice::getSupportedFeatures()
{
    qDebug("Called %s", Q_FUNC_INFO);
    return features;
}

QHash<RazerLedId, RazerLED *> RazerDevice::getLeds()
{
    return leds;
}

QList<QDBusObjectPath> RazerDevice::getLedObjectPaths()
{
    QList<QDBusObjectPath> paths;
    foreach (RazerLED *led, leds) {
        paths.append(led->getObjectPath());
    }
    return paths;
}

bool RazerDevice::hasQuirk(RazerDeviceQuirks quirk)
{
    return quirks.contains(quirk);
}

QString RazerDevice::getSerial()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!serial.isEmpty()) {
        return serial;
    }

    razer_report report, response_report;

    report = razer_chroma_standard_get_serial();
    if (!sendReportDBusHelper(report, &response_report))
        return "error";

    serial = QString((char *)&response_report.arguments[0]);
    return serial;
}

QString RazerDevice::getFirmwareVersion()
{
    qDebug("Called %s", Q_FUNC_INFO);
    razer_report report, response_report;

    report = razer_chroma_standard_get_firmware_version();
    if (!sendReportDBusHelper(report, &response_report))
        return "error";

    return QString("v%1.%2").arg(response_report.arguments[0]).arg(response_report.arguments[1]);
}

QString RazerDevice::getKeyboardLayout()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("keyboard_layout"))
        return "error";
    razer_report report, response_report;

    report = razer_chroma_standard_get_keyboard_layout();
    if (!sendReportDBusHelper(report, &response_report))
        return "error";

    qDebug("Keyboard layout ID: 0x%02X", response_report.arguments[0]);
    return keyboardLayoutIds.value(response_report.arguments[0], "unknown");
}

RazerDPI RazerDevice::getDPI()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("dpi"))
        return { 0, 0 };
    razer_report report, response_report;

    if (hasQuirk(RazerDeviceQuirks::ByteDPI)) {
        report = razer_chroma_misc_get_dpi_xy_byte();
    } else {
        report = razer_chroma_misc_get_dpi_xy(RazerVarstore::STORE);
    }
    if (!sendReportDBusHelper(report, &response_report))
        return { 0, 0 };

    ushort dpi_x, dpi_y;
    if (hasQuirk(RazerDeviceQuirks::ByteDPI)) {
        dpi_x = ((float)response_report.arguments[0]) / 255 * 6750;
        dpi_y = ((float)response_report.arguments[1]) / 255 * 6750;
    } else {
        dpi_x = (response_report.arguments[1] << 8) | (response_report.arguments[2] & 0xFF);
        dpi_y = (response_report.arguments[3] << 8) | (response_report.arguments[4] & 0xFF);
    }
    return { dpi_x, dpi_y };
}

bool RazerDevice::setDPI(RazerDPI dpi)
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("dpi"))
        return false;
    razer_report report, response_report;

    if (hasQuirk(RazerDeviceQuirks::ByteDPI)) {
        uchar dpi_x_scaled = ((float)dpi.dpi_x / 6750 * 255);
        uchar dpi_y_scaled = ((float)dpi.dpi_y / 6750 * 255);
        report = razer_chroma_misc_set_dpi_xy_byte(dpi_x_scaled, dpi_y_scaled);
    } else {
        report = razer_chroma_misc_set_dpi_xy(RazerVarstore::STORE, dpi.dpi_x, dpi.dpi_y);
    }
    if (!sendReportDBusHelper(report, &response_report))
        return false;

    return true;
}

ushort RazerDevice::getMaxDPI()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("dpi"))
        return 0;
    return maxDPI;
}

ushort RazerDevice::getPollRate()
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("poll_rate"))
        return 0;
    razer_report report, response_report;

    report = razer_chroma_misc_get_polling_rate();
    if (!sendReportDBusHelper(report, &response_report))
        return 0;

    ushort poll_rate = 0;
    switch (response_report.arguments[0]) {
    case 0x01:
        poll_rate = 1000;
        break;
    case 0x02:
        poll_rate = 500;
        break;
    case 0x08:
        poll_rate = 125;
        break;
    default: // something else
        dbusFailedHelper("Unknown poll rate returned from device.");
        return 0;
    }
    return poll_rate;
}

bool RazerDevice::setPollRate(ushort poll_rate)
{
    qDebug("Called %s", Q_FUNC_INFO);
    if (!checkFeature("poll_rate"))
        return false;
    razer_report report, response_report;

    uchar poll_rate_byte = 0x00;
    switch (poll_rate) {
    case 1000:
        poll_rate_byte = 0x01;
        break;
    case 500:
        poll_rate_byte = 0x02;
        break;
    case 125:
        poll_rate_byte = 0x08;
        break;
    default: // something else
        dbusFailedHelper("Invalid poll rate.");
        return false;
    }

    report = razer_chroma_misc_set_polling_rate(poll_rate_byte);
    if (!sendReportDBusHelper(report, &response_report))
        return false;

    return true;
}

MatrixDimensions RazerDevice::getMatrixDimensions()
{
    qDebug("Called %s", Q_FUNC_INFO);
    return matrixDimensions;
}

bool RazerDevice::startCustomEffectThread(QString effectName)
{
    if (!thread->startThread(effectName)) {
        dbusFailedHelper("Failed to start custom effect thread.");
        return false;
    }
    return true;
}

void RazerDevice::pauseCustomEffectThread()
{
    thread->pauseThread();
}

bool RazerDevice::checkFx(QString fxStr)
{
    if (!fxStr.isEmpty() && !fx.contains(fxStr)) {
        dbusNotSupportedHelper("Unsupported FX.");
        return false;
    }
    return true;
}

bool RazerDevice::checkFeature(QString featureStr)
{
    if (!features.contains(featureStr)) {
        dbusNotSupportedHelper("Unsupported feature.");
        return false;
    }
    return true;
}

void RazerDevice::customRgbDataReady(uchar row, uchar startColumn, uchar endColumn, const QByteArray &rgbData)
{
    if (!defineCustomFrame(row, startColumn, endColumn, rgbData)) {
        qWarning("defineCustomFrame went wrong.");
    }
}

void RazerDevice::customFrameReady()
{
    if (!displayCustomFrame()) {
        qWarning("displayCustomFrame went wrong.");
    }
}

bool RazerDevice::sendReportDBusHelper(razer_report request_report, razer_report *response_report)
{
    QString errorMessage;
    if (!sendReport(request_report, response_report, errorMessage)) {
        qCritical("%s", qUtf8Printable(errorMessage));
        if (calledFromDBus())
            sendErrorReply(QDBusError::Failed, qUtf8Printable(errorMessage));
        return false;
    }
    return true;
}

void RazerDevice::dbusFailedHelper(const QString &errorMessage)
{
    qWarning("%s", qUtf8Printable(errorMessage));
    if (calledFromDBus())
        sendErrorReply(QDBusError::Failed, qUtf8Printable(errorMessage));
}

void RazerDevice::dbusNotSupportedHelper(const QString &errorMessage)
{
    qWarning("%s", qUtf8Printable(errorMessage));
    if (calledFromDBus())
        sendErrorReply(QDBusError::NotSupported, qUtf8Printable(errorMessage));
}
