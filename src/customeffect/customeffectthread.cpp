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

#include <QVector>
#include <QDebug>

#include "customeffectthread.h"

CustomEffectThread::CustomEffectThread(QObject *parent) : QThread(parent)
{
    pause = false;
    abort = false;
}

CustomEffectThread::~CustomEffectThread()
{
    mutex.lock();
    abort = true;
    mutex.unlock();

    wait();
}

void CustomEffectThread::startThread()
{
    if (!isRunning()) {
        qDebug("Starting thread");
        start(LowPriority);
    } else {
        qDebug("Resuming thread?");
        pause = false;
        condition.wakeOne();
    }
}

void CustomEffectThread::pauseThread()
{
    mutex.lock();
    pause = true;
    mutex.unlock();
}

uchar increaseByNoOverflow(uchar num, uchar incBy)
{
    if ((num + incBy) > 0xFF)
        return 0xFF;
    return num + incBy;
}

uchar decreaseByNoUnderflow(uchar num, uchar decBy)
{
    if ((num - decBy) < 0x00)
        return 0x00;
    return num - decBy;
}

enum class SpectrumColor {
    Red, Yellow, Green, Cyan, Blue, Magenta
};

void CustomEffectThread::run()
{
    const uchar width = 22;
    const uchar height = 6;
    QVector<QByteArray> rgbData;
    // Initialize rgbData
    // We need height (=6) QByteArrays
    rgbData.resize(height);
    foreach (QByteArray arr, rgbData) {
        // And each needs space for width * 3 bytes of data (RGB)
        arr.resize(width * 3);
    }

    RGBval startVal = {0xFF, 0x00, 0x00}; // Red
    SpectrumColor startNextColor = SpectrumColor::Yellow;

    forever {
        if (abort)
            return;

        // Iterate through rows
        for (uchar i = 0; i < height; i++) {
            RGBval rowVal = startVal;
            SpectrumColor nextColor = startNextColor;
            // Iterate through columns
            for (int j = 0; j < width * 3; j++) {
                rgbData[i][j++] = rowVal.red;
                rgbData[i][j++] = rowVal.green;
                rgbData[i][j] = rowVal.blue;

                // FF0000 Red
                // Increase Green until
                // FFFF00 Yellow
                // Decrease Red until
                // 00FF00 Green
                // Increase Blue until
                // 00FFFF Cyan
                // Decrease Green until
                // 0000FF Blue
                // Increase Red until
                // FF00FF Magenta
                // Decrease Blue until
                // FF0000 Red
                // REPEAT
                if (nextColor == SpectrumColor::Yellow) {
                    rowVal.green = increaseByNoOverflow(rowVal.green, 0x40);
                    if (rowVal.green == 0xFF)
                        nextColor = SpectrumColor::Green;
                } else if (nextColor == SpectrumColor::Green) {
                    rowVal.red = decreaseByNoUnderflow(rowVal.red, 0x40);
                    if (rowVal.red == 0x00)
                        nextColor = SpectrumColor::Cyan;
                } else if (nextColor == SpectrumColor::Cyan) {
                    rowVal.blue = increaseByNoOverflow(rowVal.blue, 0x40);
                    if (rowVal.blue == 0xFF)
                        nextColor = SpectrumColor::Blue;
                } else if (nextColor == SpectrumColor::Blue) {
                    rowVal.green = decreaseByNoUnderflow(rowVal.green, 0x40);
                    if (rowVal.green == 0x00)
                        nextColor = SpectrumColor::Magenta;
                } else if (nextColor == SpectrumColor::Magenta) {
                    rowVal.red = increaseByNoOverflow(rowVal.red, 0x40);
                    if (rowVal.red == 0xFF)
                        nextColor = SpectrumColor::Red;
                } else if (nextColor == SpectrumColor::Red) {
                    rowVal.blue = decreaseByNoUnderflow(rowVal.blue, 0x40);
                    if (rowVal.blue == 0x00)
                        nextColor = SpectrumColor::Yellow;
                }
            }
            // Send data to keyboard
            emit rgbDataReady(i, 0, width - 1, rgbData[i]);

            // Last row - keep current data for next frame
            if (i == height - 1) {
                startVal = rowVal;
                startNextColor = nextColor;
            }
        }
        // Show the frame
        emit frameReady();

        QThread::msleep(100); // 0.1 seconds

        mutex.lock();
        if (pause)
            condition.wait(&mutex);
        pause = false;
        mutex.unlock();
    }
}
