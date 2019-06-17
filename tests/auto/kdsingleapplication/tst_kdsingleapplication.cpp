/*
    MIT License

    Copyright (C) 2019 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com

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
#include <QtTest/QtTest>

#include <QDebug>

#include <vector>
#include <memory>

class tst_KDSingleApplication : public QObject
{
    Q_OBJECT
private slots:
    void testSimple_data();
    void testSimple();

    void testOnePrimaryManySecondaries();

    void testOnlyOnePrimary_data();
    void testOnlyOnePrimary();
};

void tst_KDSingleApplication::testSimple_data()
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

void tst_KDSingleApplication::testSimple()
{
    QFETCH(QString, message);

    const QString executable = QStringLiteral("simpletest/simpletest");
    QByteArray output;
    bool ok;

    QProcess primary;
    primary.setProcessChannelMode(QProcess::ForwardedErrorChannel);
    primary.start(executable);
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
    secondary.start(executable, { message });
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

void tst_KDSingleApplication::testOnePrimaryManySecondaries()
{
    // Each secondary will output runsPerSecondary _blocks_ of messages;
    // we run secondariesCount secondaries.
    // * The primary gets: how many blocks of messages to expect in total
    // * The secondaries get: how many blocks to send
    //
    // (The primary will do the math internally)

    const QString executable = QStringLiteral("stresstest/stresstest");
    const int secondariesCount = 50;
    const int runsPerSecondary = 50;

    QByteArray output;
    bool ok;

    QProcess primary;
    primary.setProcessChannelMode(QProcess::ForwardedErrorChannel);
    primary.start(executable, { QStringLiteral("primary"), QString::number(secondariesCount * runsPerSecondary) });
    QVERIFY(primary.waitForStarted());
    QCOMPARE(primary.state(), QProcess::Running);
    output.clear();
    ok = QTest::qWaitFor([&](){
        output += primary.readAllStandardOutput();
        return output == "Primary\n";
    });
    QVERIFY(ok);

    std::vector<std::unique_ptr<QProcess>> secondaries;
    secondaries.reserve(secondariesCount);
    for (int i = 0; i < secondariesCount; ++i) {
        std::unique_ptr<QProcess> secondary = std::make_unique<QProcess>();
        secondary->setProcessChannelMode(QProcess::ForwardedErrorChannel);
        secondary->start(executable, { QStringLiteral("secondary"), QString::number(runsPerSecondary) });
        secondaries.push_back(std::move(secondary));
    }

    for (auto &secondary : secondaries)
        QVERIFY(secondary->waitForStarted());

    for (auto &secondary : secondaries) {
        if (secondary->state() != QProcess::NotRunning)
            QVERIFY(secondary->waitForFinished());
        QCOMPARE(secondary->exitCode(), 0);
        QCOMPARE(secondary->exitStatus(), QProcess::NormalExit);
    }

    secondaries.clear();

    if (primary.state() != QProcess::NotRunning)
        QVERIFY(primary.waitForFinished());
    QCOMPARE(primary.exitCode(), 0);
    QCOMPARE(primary.exitStatus(), QProcess::NormalExit);
}

void tst_KDSingleApplication::testOnlyOnePrimary_data()
{
    QTest::addColumn<int>("count");
    for (int i = 10; i <= 100; i += 10)
        QTest::addRow("count-%d", i) << i;
}

void tst_KDSingleApplication::testOnlyOnePrimary()
{
    QFETCH(int, count);

    int primaryCount = 0;
    int secondaryCount = 0;

    const QString executable = QStringLiteral("stresstest2/stresstest2");
    const QString countString = QString::number(count);

    std::vector<std::unique_ptr<QProcess>> processes;
    processes.reserve(count);
    for (int i = 0; i < count; ++i) {
        std::unique_ptr<QProcess> process = std::make_unique<QProcess>();
        process->setProcessChannelMode(QProcess::ForwardedErrorChannel);
        process->start(executable, { countString });
        processes.push_back(std::move(process));
    }

    for (auto &process : processes) {
        if (process->state() != QProcess::NotRunning)
            QVERIFY(process->waitForFinished());
        QCOMPARE(process->exitCode(), 0);
        QCOMPARE(process->exitStatus(), QProcess::NormalExit);

        QByteArray output = process->readAllStandardOutput();
        if (output == "Primary\n")
            ++primaryCount;
        else if (output == "Secondary\n")
            ++secondaryCount;
        else
            QFAIL("Unexpected output");
    }

    QCOMPARE(primaryCount, 1);
    QCOMPARE(secondaryCount, count - 1);
}

QTEST_MAIN(tst_KDSingleApplication)

#include "tst_kdsingleapplication.moc"
