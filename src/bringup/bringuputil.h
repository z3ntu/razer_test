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

#ifndef BRINGUPUTIL_H
#define BRINGUPUTIL_H

#include <hidapi.h>

#include "../device/razerdevice.h"

/**
 * @todo write docs
 */
class BringupUtil
{
public:
    BringupUtil(struct hid_device_info *hid_dev_info);
    BringupUtil(RazerDevice *device);
    bool newDevice();

    // Automatic
    bool testDPI();
    bool testPollRate();
    bool testKeyboardLayout();
    bool testBrightness();

    // Interactive
    QVector<RazerEffect> testLedEffects();

private:
    struct hid_device_info *hid_dev_info;
    RazerDevice *device;

    bool setEffect(RazerLED *led, razer_test::RazerEffect effect, razer_test::RGB color1, razer_test::RGB color2, razer_test::RGB color3);
};

#endif // BRINGUPUTIL_H
