/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2019 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include <QtCore/QProcess>
#include <QtCore/QRandomGenerator>
#include <QtTest/QTest>


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
    // QTest::addRow("message empty") << QString();

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
    ok = QTest::qWaitFor([&]() {
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
    ok = QTest::qWaitFor([&]() {
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
    ok = QTest::qWaitFor([&]() {
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
