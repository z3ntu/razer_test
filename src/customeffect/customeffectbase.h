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

#ifndef CUSTOMEFFECTBASE_H
#define CUSTOMEFFECTBASE_H

#include <QObject>
#include <QVector>

struct RGBval {
    uchar red;
    uchar green;
    uchar blue;
};

enum class SpectrumColor {
    Red, Yellow, Green, Cyan, Blue, Magenta
};

inline uchar increaseByNoOverflow(uchar num, uchar incBy)
{
    if ((num + incBy) > 0xFF)
        return 0xFF;
    return num + incBy;
}

inline uchar decreaseByNoUnderflow(uchar num, uchar decBy)
{
    if ((num - decBy) < 0x00)
        return 0x00;
    return num - decBy;
}

/**
 * @todo write docs
 */
class CustomEffectBase : public QObject
{
    Q_OBJECT
public:
    CustomEffectBase(uchar width, uchar height);

    virtual void initialize() = 0;
    virtual void prepareRgbData() = 0;

    ulong msleep;

signals:
    void rgbDataReady(const uchar row, const QByteArray &rgbData);

protected:
    const uchar width;
    const uchar height;
    QVector<QByteArray> rgbData;
};

#endif // CUSTOMEFFECTBASE_H
