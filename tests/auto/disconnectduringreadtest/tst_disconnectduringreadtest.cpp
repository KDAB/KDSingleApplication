/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2026 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include <QtCore/QProcess>
#include <QtCore/QRandomGenerator>
#include <QtTest/QTest>


// Tests that processing events inside a messageReceived handler
// (e.g. showing a QMessageBox) does not crash when the secondary
// disconnects while the readyRead handler is still on the stack.

class tst_DisconnectDuringReadTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testDisconnectDuringRead();
};

void tst_DisconnectDuringReadTest::testDisconnectDuringRead()
{
#ifdef KDSINGLEAPPLICATION_BINARY_DIR
    const QString executable = QStringLiteral(KDSINGLEAPPLICATION_BINARY_DIR "disconnectduringreadtest");
#else
    const QString executable = QStringLiteral("disconnectduringreadtest/disconnectduringreadtest");
#endif
    QByteArray output;
    bool ok;

    const QString testId = QString::number(QRandomGenerator::global()->generate());
    const QString message = QStringLiteral("Hello");

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
    QVERIFY2(ok, "Primary instance likely crashed before printing the message");

    if (primary.state() != QProcess::NotRunning)
        QVERIFY(primary.waitForFinished());
    QCOMPARE(primary.exitCode(), 0);
    QCOMPARE(primary.exitStatus(), QProcess::NormalExit);
}

QTEST_MAIN(tst_DisconnectDuringReadTest)

#include "tst_disconnectduringreadtest.moc"
