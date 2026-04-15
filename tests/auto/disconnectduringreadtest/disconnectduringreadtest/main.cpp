/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2019 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

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

    const QString appName = QLatin1String("disconnectduringreadtest-") + QCoreApplication::arguments().value(1);

    KDSingleApplication kdsa(appName);

    if (kdsa.isPrimaryInstance()) {
        std::cout << "Primary" << std::endl;

        QObject::connect(&kdsa, &KDSingleApplication::messageReceived, qApp,
                         [](const QByteArray &message) {
                             // Spin the event loop, simulating what QMessageBox::information() does.
                             // This causes the socket's disconnected signal to be processed
                             // while we're still inside readDataFromSecondarySocket(),
                             // leading to a use-after-free crash.
                             QCoreApplication::processEvents();

                             std::cout << "MESSAGE: >" << message.constData() << '<' << std::endl;
                             qApp->quit();
                         });

        QTimer::singleShot(std::chrono::seconds(5), qApp, [] { qApp->exit(1); });
        return QCoreApplication::exec();
    }

    std::cout << "Secondary" << std::endl;

    if (!kdsa.sendMessage(QCoreApplication::arguments().value(2).toUtf8())) {
        std::cerr << "Unable to send message to the primary!" << '\n';
        return 1;
    }

    return 0;
}
