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
    startVal = { 0xFF, 0x00, 0x00 }; // Red
    startNextColor = SpectrumColor::Yellow;
    msleep = 100;
}

void WaveEffect::prepareRgbData()
{
    // Iterate through rows
    for (uchar i = 0; i < height; i++) {
        openrazer::RGB rowVal = startVal;
        SpectrumColor nextColor = startNextColor;
        // Iterate through columns
        for (int j = 0; j < width; j++) {
            rgbData[i][j] = rowVal;

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
                rowVal.g = increaseByNoOverflow(rowVal.g, 0x40);
                if (rowVal.g == 0xFF)
                    nextColor = SpectrumColor::Green;
            } else if (nextColor == SpectrumColor::Green) {
                rowVal.r = decreaseByNoUnderflow(rowVal.r, 0x40);
                if (rowVal.r == 0x00)
                    nextColor = SpectrumColor::Cyan;
            } else if (nextColor == SpectrumColor::Cyan) {
                rowVal.b = increaseByNoOverflow(rowVal.b, 0x40);
                if (rowVal.b == 0xFF)
                    nextColor = SpectrumColor::Blue;
            } else if (nextColor == SpectrumColor::Blue) {
                rowVal.g = decreaseByNoUnderflow(rowVal.g, 0x40);
                if (rowVal.g == 0x00)
                    nextColor = SpectrumColor::Magenta;
            } else if (nextColor == SpectrumColor::Magenta) {
                rowVal.r = increaseByNoOverflow(rowVal.r, 0x40);
                if (rowVal.r == 0xFF)
                    nextColor = SpectrumColor::Red;
            } else if (nextColor == SpectrumColor::Red) {
                rowVal.b = decreaseByNoUnderflow(rowVal.b, 0x40);
                if (rowVal.b == 0x00)
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
