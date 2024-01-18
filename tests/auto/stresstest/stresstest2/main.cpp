/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2019 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include <QtCore/QCoreApplication>
#include <QtCore/QRandomGenerator>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QThread>

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

    const int delay = ( int )QRandomGenerator::global()->bounded(100, 200);
    QThread::msleep(delay);

    const QString appName = QLatin1String("stresstest2-") + app.arguments().value(1);
    const int timeout = app.arguments().value(2).toInt();

    KDSingleApplication kdsa(appName);
    if (kdsa.isPrimaryInstance()) {
        std::cout << "Primary" << std::endl;

        int counter = app.arguments().value(3).toInt();
        --counter;

        QObject::connect(&kdsa, &KDSingleApplication::messageReceived,
                         [&counter]() {
                             --counter;
                             if (counter == 0)
                                 qApp->quit();
                         });

        QTimer::singleShot(timeout, [&counter]() {
            std::cerr << "Primary time out, still " << counter << " secondaries" << std::endl;
            qApp->exit(1);
        });

        return app.exec();
    } else {
        std::cout << "Secondary" << std::endl;

        if (!kdsa.sendMessageWithTimeout(QByteArray(delay, 'x'), timeout))
            return 1;
    }

    return 0;
}
