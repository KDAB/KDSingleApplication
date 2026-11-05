/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2026 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QTimer>

#include <kdsingleapplication.h>

#include <iostream>

#if defined(Q_OS_WIN)
#include <io.h>
#include <fcntl.h>
#endif

// Usage: waitforprimaryexittest <id> <mode> [timeout]
//
// As primary: prints "Primary" and quits when it receives the "quit" message.
// As secondary, depending on <mode>:
//   "wait": blocks until the primary exits (or [timeout] expires);
//           exits with 0 if the primary exited, 2 on timeout
//   "quit": tells the primary to quit
int main(int argc, char **argv)
{
#if defined(Q_OS_WIN)
    _setmode(_fileno(stdout), _O_BINARY);
#endif

    QCoreApplication app(argc, argv);

    const QString appName = QLatin1String("waitforprimaryexittest-") + app.arguments().value(1);
    const QString mode = app.arguments().value(2);

    KDSingleApplication kdsa(appName);

    if (kdsa.isPrimaryInstance()) {
        std::cout << "Primary" << std::endl;

        QObject::connect(&kdsa, &KDSingleApplication::messageReceived, qApp,
                         [](const QByteArray &message) {
                             if (message == "quit")
                                 qApp->quit();
                         });

        QTimer::singleShot(30000, qApp, []() { qApp->exit(1); });

        return app.exec();
    }

    std::cout << "Secondary" << std::endl;

    if (mode == QLatin1String("wait")) {
        bool ok = false;
        const int timeout = app.arguments().value(3).toInt(&ok);
        const bool exited = ok ? kdsa.waitForPrimaryInstanceToExit(timeout)
                               : kdsa.waitForPrimaryInstanceToExit();
        if (!exited) {
            std::cout << "Timeout" << std::endl;
            return 2;
        }
        std::cout << "PrimaryExited" << std::endl;
        return 0;
    }

    if (mode == QLatin1String("quit")) {
        if (!kdsa.sendMessage("quit")) {
            std::cerr << "Unable to send message to the primary!" << std::endl;
            return 1;
        }
        return 0;
    }

    std::cerr << "Unknown mode" << std::endl;
    return 1;
}
