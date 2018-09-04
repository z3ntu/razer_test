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

#include "razerled.h"

// Marshall the RazerLedId data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const RazerLedId &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the RazerLedId data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RazerLedId &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<RazerLedId>(a);
    return argument;
}

// Marshall the WaveDirection data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const WaveDirection &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the WaveDirection data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, WaveDirection &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<WaveDirection>(a);
    return argument;
}

// Marshall the ReactiveSpeed data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const ReactiveSpeed &value)
{
    argument.beginStructure();
    argument << static_cast<int>(value);
    argument.endStructure();
    return argument;
}

// Retrieve the ReactiveSpeed data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, ReactiveSpeed &value)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    value = static_cast<ReactiveSpeed>(a);
    return argument;
}

RazerLED::RazerLED(RazerLedId ledId)
{
    this->ledId = ledId;
}

RazerLED::~RazerLED()
{
}
