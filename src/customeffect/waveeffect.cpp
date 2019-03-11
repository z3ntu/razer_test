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

#include "waveeffect.h"

void WaveEffect::initialize()
{
    startVal = {0xFF, 0x00, 0x00}; // Red
    startNextColor = SpectrumColor::Yellow;
    msleep = 100;
}

void WaveEffect::prepareRgbData()
{
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
        emit rgbDataReady(i, rgbData[i]);

        // Last row - keep current data for next frame
        if (i == height - 1) {
            startVal = rowVal;
            startNextColor = nextColor;
        }
    }
}
