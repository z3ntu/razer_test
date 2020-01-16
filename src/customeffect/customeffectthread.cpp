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

#include "customeffectthread.h"

#include "spectrumeffect.h"
#include "waveeffect.h"

#include <QVector>

CustomEffectThread::CustomEffectThread(const uchar width, const uchar height, QObject *parent)
    : QThread(parent), width(width), height(height)
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

bool CustomEffectThread::startThread(QString effectName)
{
    if (isRunning() && !pause) {
        qWarning("Effect thread is already running. Pause it first");
        return false;
    }

    // Only recreate instances when the effect has actually changed
    if (currentEffect != effectName) {
        // Delete previous effect class
        delete customEffect;
        currentEffect = effectName;
        if (effectName == "spectrum") {
            customEffect = new SpectrumEffect(width, height);
        } else if (effectName == "wave") {
            customEffect = new WaveEffect(width, height);
        } else {
            qWarning("Effect %s unknown.", qUtf8Printable(effectName));
            return false;
        }
        connect(customEffect, &CustomEffectBase::rgbDataReady, this, &CustomEffectThread::customEffectRgbDataReady);
        customEffect->initialize();
    }

    if (!isRunning()) {
        qDebug("Starting custom effect thread.");
        start(LowPriority);
    } else {
        qDebug("Resuming custom effect thread.");
        pause = false;
        condition.wakeOne();
    }
    return true;
}

void CustomEffectThread::pauseThread()
{
    mutex.lock();
    pause = true;
    mutex.unlock();
}

void CustomEffectThread::run()
{
    forever {
        if (abort)
            return;

        customEffect->prepareRgbData();

        // Show the frame
        emit frameReady();

        QThread::msleep(customEffect->msleep);

        mutex.lock();
        if (pause)
            condition.wait(&mutex);
        pause = false;
        mutex.unlock();
    }
}

void CustomEffectThread::customEffectRgbDataReady(const uchar row, const QVector<RGB> &rgbData)
{
    emit rgbDataReady(row, 0, width - 1, rgbData);
}
