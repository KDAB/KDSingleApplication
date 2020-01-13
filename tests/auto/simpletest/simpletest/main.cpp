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

    const QString appName = QLatin1String("simpletest-") + app.arguments().at(1);

    KDSingleApplication kdsa(appName);

    if (kdsa.isPrimaryInstance()) {
        std::cout << "Primary" << std::endl;

        QObject::connect(&kdsa, &KDSingleApplication::messageReceived,
                         [](const QString &message) {
            std::cout << "MESSAGE: >"  << qPrintable(message) << '<' << std::endl;
            qApp->quit();
        });

        QTimer::singleShot(5000, [](){ qApp->exit(1); });

        return app.exec();
    } else {
        std::cout << "Secondary" << std::endl;

        if (!kdsa.sendMessage(app.arguments().at(2))) {
            std::cerr << "Unable to send message to the primary!" << std::endl;
            return 1;
        }
    }

    return 0;
}
