/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>

#include <kdsingleapplication.h>

#include <chrono>
#include <iostream>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    KDSingleApplication kdsa;

    QTimer shutdownTimer;
    shutdownTimer.setSingleShot(true);
    shutdownTimer.setInterval(std::chrono::seconds(15));
    QObject::connect(&shutdownTimer, &QTimer::timeout, &app, &QCoreApplication::quit);

    if (kdsa.isPrimaryInstance()) {
        std::cout << "Primary; waiting for secondary instances..." << std::endl;
        shutdownTimer.start();

        QObject::connect(&kdsa, &KDSingleApplication::messageReceived,
                         [&shutdownTimer](const QByteArray &message) {
                             shutdownTimer.start();
                             std::cout << "Message from secondary: >" << message.constData() << '<' << std::endl;
                         });
    } else {
        auto args = app.arguments();
        const QByteArray message = args.join(QLatin1Char(',')).toUtf8();

        std::cout << "Secondary; sending message >" << message.constData() << '<' << std::endl;

        if (!kdsa.sendMessage(message)) {
            std::cerr << "Unable to send message to the primary!" << std::endl;
            return 1;
        }

        return 0;
    }

    return app.exec();
}
