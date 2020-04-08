/*
    MIT License

    Copyright (C) 2019-2020 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
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
            std::cout << "Message from secondary: >"  << message.constData() << '<' << std::endl;
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
