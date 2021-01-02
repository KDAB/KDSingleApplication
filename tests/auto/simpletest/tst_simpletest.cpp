/*
    MIT License

    Copyright (C) 2019-2021 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com

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

#include <QtCore/QProcess>
#include <QtCore/QRandomGenerator>
#include <QtTest/QtTest>


#include <vector>
#include <memory>

// runs a primary, runs a secondary, checks that a message is received.

class tst_SimpleTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSimple_data();
    void testSimple();
};

void tst_SimpleTest::testSimple_data()
{
    QTest::addColumn<QString>("message");

    QTest::addRow("message-1") << QStringLiteral("Hello");
    QTest::addRow("message-2") << QStringLiteral("Hello World");
    QTest::addRow("message-3") << QStringLiteral("Hello World 123456789");
    //QTest::addRow("message empty") << QString();

    for (int i = 1; i <= 2048; i *= 2) {
        QTest::addRow("message-x-%d", i) << QString(i, QLatin1Char('x'));
    }
}

void tst_SimpleTest::testSimple()
{
    QFETCH(QString, message);
#ifdef KDSINGLEAPPLICATION_BINARY_DIR
    const QString executable = QStringLiteral(KDSINGLEAPPLICATION_BINARY_DIR "simpletest");
#else
    const QString executable = QStringLiteral("simpletest/simpletest");
#endif
    QByteArray output;
    bool ok;

    const QString testId = QString::number(QRandomGenerator::global()->generate());

    QProcess primary;
    primary.setProcessChannelMode(QProcess::ForwardedErrorChannel);
    primary.start(executable, { testId });
    QVERIFY(primary.waitForStarted());
    QCOMPARE(primary.state(), QProcess::Running);
    output.clear();
    ok = QTest::qWaitFor([&](){
        output += primary.readAllStandardOutput();
        return output == "Primary\n";
    });
    QVERIFY(ok);

    QProcess secondary;
    secondary.setProcessChannelMode(QProcess::ForwardedErrorChannel);
    secondary.start(executable, { testId, message });
    QVERIFY(secondary.waitForStarted());
    QCOMPARE(secondary.state(), QProcess::Running);

    output.clear();
    ok = QTest::qWaitFor([&](){
        output += secondary.readAllStandardOutput();
        return output == "Secondary\n";
    });
    QVERIFY(ok);

    if (secondary.state() != QProcess::NotRunning)
        QVERIFY(secondary.waitForFinished());
    QCOMPARE(secondary.exitCode(), 0);
    QCOMPARE(secondary.exitStatus(), QProcess::NormalExit);

    output.clear();
    const QByteArray expected = "MESSAGE: >" + message.toUtf8() + "<\n";
    ok = QTest::qWaitFor([&](){
        output += primary.readAllStandardOutput();
        return output == expected;
    });
    QVERIFY(ok);

    if (primary.state() != QProcess::NotRunning)
        QVERIFY(primary.waitForFinished());
    QCOMPARE(primary.exitCode(), 0);
    QCOMPARE(primary.exitStatus(), QProcess::NormalExit);
}

QTEST_MAIN(tst_SimpleTest)

#include "tst_simpletest.moc"
