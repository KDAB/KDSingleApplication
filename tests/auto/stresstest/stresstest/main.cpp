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

    const QStringList messages = {
        QStringLiteral("secondary"),
        QStringLiteral("secondary 123456"),
        QString(1024, QLatin1Char('x'))
    };

    const QString appName = QLatin1String("stresstest-") + app.arguments().at(1);
    const QString mode = app.arguments().at(2);
    const int counter = app.arguments().at(3).toInt();

    if (mode == QLatin1String("primary")) {
        KDSingleApplication kdsa(appName);
        if (!kdsa.isPrimaryInstance())
            return 1;

        std::cout << "Primary" << std::endl;

        int totalMessages = counter * messages.size();

        QObject::connect(&kdsa, &KDSingleApplication::messageReceived,
                         [&totalMessages]()
        {
            if (--totalMessages == 0)
                qApp->quit();
        });

        QTimer::singleShot(30000, [&totalMessages](){
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
                if (!kdsa.sendMessage(message))
                    return 2;
            }
        }
    }

    return 0;
}
