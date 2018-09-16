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

#ifndef CUSTOMEFFECTTHREAD_H
#define CUSTOMEFFECTTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "customeffectbase.h"

/**
 * @todo write docs
 */
class CustomEffectThread : public QThread
{
    Q_OBJECT
public:
    CustomEffectThread(QObject *parent = nullptr);
    ~CustomEffectThread() override;

    bool startThread(QString effectName);
    void pauseThread();

signals:
    void rgbDataReady(uchar row, uchar startColumn, uchar endColumn, const QByteArray &rgbData);
    void frameReady();

protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition condition;
    bool pause;
    bool abort;

    // TODO Height and width is device-specific
    const uchar width = 22;
    const uchar height = 6;

    CustomEffectBase *customEffect = nullptr;
    QString currentEffect;

private slots:
    void customEffectRgbDataReady(const uchar row, const QByteArray &rgbData);
};

#endif // CUSTOMEFFECTTHREAD_H
