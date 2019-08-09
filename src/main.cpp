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

#include "config.h"
#include "daemon.h"
#include "razer_test.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDir>

#define ANSI_BOLD "\x1b[1m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_RESET "\x1b[0m"

// Used to tell myMessageOutput if --verbose was given on the command line
bool verbose = false;

void myMessageOutput(QtMsgType type, const QMessageLogContext & /*context*/, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        if (verbose)
            fprintf(stderr, ANSI_BOLD ANSI_COLOR_GREEN "[debug]" ANSI_RESET " %s\n", localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stderr, ANSI_BOLD ANSI_COLOR_GREEN "[info]" ANSI_RESET " %s\n", localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, ANSI_BOLD ANSI_COLOR_YELLOW "[warning]" ANSI_RESET " %s\n", localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, ANSI_BOLD ANSI_COLOR_RED "[critical]" ANSI_RESET " %s\n", localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, ANSI_BOLD ANSI_COLOR_RED "[fatal]" ANSI_RESET " %s\n", localMsg.constData());
        break;
    }
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_DARWIN
    // For Device Notifier: Lets Qt use the CF loop, otherwise CFRunLoopRun() would have to be used.
    qputenv("QT_EVENT_DISPATCHER_CORE_FOUNDATION", "1");
#endif

    QCoreApplication app(argc, argv);

    // Set the directory of the application to where the application is located. Needed for relative paths.
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    QCoreApplication::setApplicationName("razer_test");
    QCoreApplication::setApplicationVersion(RAZER_TEST_VERSION);

    // Setup up the command line parser
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption({ "devel", QString("Uses data files at ../data/devices instead of %1.").arg(RAZER_TEST_DATADIR) });
    parser.addOption({ "fake-devices", "Adds fake devices instead of real ones." });
    parser.addOption({ "verbose", "Print debug messages." });
    parser.process(app);

    verbose = parser.isSet("verbose");
    qInstallMessageHandler(myMessageOutput);

#ifdef ENABLE_BRINGUP_UTIL
    const char *bringup = " (bringup util)";
#else
    const char *bringup = "";
#endif
    qInfo("razer_test%s - version %s", bringup, RAZER_TEST_VERSION);
    if (parser.isSet("devel"))
        qInfo("Running in development mode and using development data files.");

    // Register the enums with the Qt system
    registerMetaTypes();

    Daemon daemon(parser.isSet("devel"), parser.isSet("fake-devices"));
    daemon.initialize();
    return app.exec();
}
