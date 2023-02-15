/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

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

int main(int argc, char **argv)
{
#if defined(Q_OS_WIN)
    _setmode(_fileno(stdout), _O_BINARY);
#endif

    QCoreApplication app(argc, argv);

    const QByteArrayList messages = {
        QByteArrayLiteral("secondary"),
        QByteArrayLiteral("secondary 123456"),
        QByteArray(1024 * 10, 'x')
    };

    const QString appName = QLatin1String("stresstest-") + app.arguments().at(1);
    const QString mode = app.arguments().at(2);
    const int timeout = app.arguments().at(3).toInt();
    const int counter = app.arguments().at(4).toInt();

    if (mode == QLatin1String("primary")) {
        KDSingleApplication kdsa(appName);
        if (!kdsa.isPrimaryInstance())
            return 1;

        std::cout << "Primary" << std::endl;

        int totalMessages = counter * messages.size();

        QObject::connect(&kdsa, &KDSingleApplication::messageReceived,
                         [&totalMessages]() {
                             if (--totalMessages == 0)
                                 qApp->quit();
                         });

        QTimer::singleShot(timeout, [&totalMessages]() {
            std::cerr << "Primary timed out, still " << totalMessages << " messages" << std::endl;
            qApp->exit(1);
        });

        return app.exec();
    } else if (mode == QLatin1String("secondary")) {
        for (int i = 0; i < counter; ++i) {
            KDSingleApplication kdsa(appName);
            if (kdsa.isPrimaryInstance())
                return 1;

            for (const auto &message : messages) {
                if (!kdsa.sendMessageWithTimeout(message, timeout))
                    return 2;
            }
        }
    }

    return 0;
}
