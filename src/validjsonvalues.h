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

#ifndef VALIDJSONVALUES_H
#define VALIDJSONVALUES_H

#include <QStringList>

const QStringList allowedKeys { "name", "vid", "pid", "type", "pclass", "leds", "fx", "features", "quirks", "matrix_dimensions", "max_dpi" };

const QStringList validType = { "accessory", "headset", "keyboard", "keypad", "mouse", "mousepad" };
const QStringList validPclass = { "classic", "matrix" };
const QStringList validFx = { "off", "static", "blinking", "breathing", "breathing_dual", "breathing_random", "spectrum", "wave", "reactive", "custom_frame", "brightness" };
const QStringList validFeatures = { "keyboard_layout", "dpi", "poll_rate" };
const QStringList validQuirks = { "mouse_matrix", "matrix_brightness", "no_get_brightness", "firefly_custom_frame" };

#endif // VALIDJSONVALUES_H
