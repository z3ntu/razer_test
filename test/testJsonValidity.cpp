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


#include <QObject>
#include <QtTest>

class testJsonValidity : public QObject
{
    Q_OBJECT
private:
    QJsonArray loadJson();
    ushort hexStringToUshort(const QString &str);

    QStringList allowedKeys = {"name", "vid", "pid", "type", "pclass", "leds", "fx", "features", "quirks"};

    QStringList validType = {"core", "headset", "keyboard", "keypad", "mouse", "mousepad", "mug"};
    QStringList validPclass = {"classic", "matrix"};
    QStringList validFx = {"off", "static", "blinking", "breathing", "breathing_dual", "breathing_random", "spectrum", "wave", "reactive", "custom_frame"};
    QStringList validFeatures = {"dpi", "poll_rate"};
    QStringList validQuirks = {"mouse_matrix", "matrix_brightness"};

private slots:
    void checkJsonDataValidity();
};

QTEST_MAIN(testJsonValidity)

QJsonArray testJsonValidity::loadJson()
{
    QJsonArray devices;

    QDir datadir("../data");
    QStringList filters;
    filters << "*.json";
    datadir.setNameFilters(filters);

    QListIterator<QFileInfo> i(datadir.entryInfoList());
    while (i.hasNext()) {
        QFile f(i.next().absoluteFilePath());
        f.open(QFile::ReadOnly);
        QJsonArray a = QJsonDocument::fromJson(f.readAll()).array();
        foreach (const QJsonValue &value, a) {
            devices.append(value);
        }
    }
    return devices;
}

ushort testJsonValidity::hexStringToUshort(const QString &str)
{
    bool ok;
    ushort num = str.toUShort(&ok, 16);
    if (!ok) return 0;
    return num;
}

void testJsonValidity::checkJsonDataValidity()
{
    QJsonArray devices = loadJson();
    QVERIFY2(devices.size() > 0, "Couldn't find test data at ../data/. Please adjust your working directory.");

    foreach (const QJsonValue &devVal, devices) {
        QJsonObject devObj = devVal.toObject();

        qDebug() << "name:" << devObj["name"].toString();
        QVERIFY2(devObj["name"].isString(), "Invalid device name - has to be a string.");
        qDebug() << "vid:" << devObj["vid"].toString();
        QVERIFY2(hexStringToUshort(devObj["vid"].toString()) == 0x1532, "Invalid vid - has to be \"1532\".");
        qDebug() << "pid:" << devObj["pid"].toString();
        QVERIFY2(hexStringToUshort(devObj["pid"].toString()) != 0, "Invalid pid - has to be a hex number in string form.");
        qDebug() << "type:" << devObj["type"].toString();
        QVERIFY2(validType.contains(devObj["type"].toString()), "Invalid device type.");
        qDebug() << "pclass:" << devObj["pclass"].toString();
        QVERIFY2(validPclass.contains(devObj["pclass"].toString()), "Invalid pclass.");
        // leds array
        QVERIFY2(devObj["leds"].isArray(), "Invalid leds - has to be an array.");
        foreach (const QJsonValue &ledVal, devObj["leds"].toArray()) {
            qDebug() << "leds:" << ledVal.toInt();
            QVERIFY2(0x00 < ledVal.toInt() && ledVal.toInt() <= 0x14, QString("Invalid leds value '%1' - has to be 0x00 < led <= 0x14.").arg(ledVal.toInt()).toLatin1());
        }
        // fx array
        QVERIFY2(devObj["fx"].isArray(), "Invalid fx - has to be an array.");
        foreach (const QJsonValue &fxVal, devObj["fx"].toArray()) {
            qDebug() << "fx:" << fxVal.toString();
            QVERIFY2(validFx.contains(fxVal.toString()), "Invalid fx.");
        }
        // features array
        QVERIFY2(devObj["features"].isArray(), "Invalid features - has to be an array.");
        foreach (const QJsonValue &featVal, devObj["features"].toArray()) {
            qDebug() << "features:" << featVal.toString();
            QVERIFY2(validFeatures.contains(featVal.toString()), "Invalid features.");
        }
        // quirks array - optional
        if (devObj["quirks"].isArray()) {
            foreach (const QJsonValue &quirkVal, devObj["quirks"].toArray()) {
                qDebug() << "quirks:" << quirkVal.toString();
                QVERIFY2(validQuirks.contains(quirkVal.toString()), "Invalid quirks.");
            }
        } else if (devObj.contains("quirks")) {
            QFAIL("Invalid quirks - has to be an array.");
        }

        // Check that all keys present in the device json are valid.
        QStringList keys = devObj.keys();
        for (int i = 0; i < keys.size(); ++i) {
            qDebug() << "key:" << keys.at(i);
            QVERIFY2(allowedKeys.contains(keys.at(i)), "Invalid key.");
        }
    }
}

#include "testJsonValidity.moc"
