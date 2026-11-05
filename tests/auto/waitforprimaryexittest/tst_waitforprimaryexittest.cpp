/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2026 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include <QtCore/QProcess>
#include <QtCore/QRandomGenerator>
#include <QtTest/QTest>

#include <memory>

// Checks that a secondary can block until the primary exits.

class tst_WaitForPrimaryExitTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void init();
    void cleanup();

    void testPrimaryQuits();
    void testPrimaryKilled();
    void testTimeout();

private:
    std::unique_ptr<QProcess> startInstance(const QStringList &args, const QByteArray &expectedOutput);

    QString m_testId;
    std::unique_ptr<QProcess> m_primary;
};

static QString executable()
{
#ifdef KDSINGLEAPPLICATION_BINARY_DIR
    return QStringLiteral(KDSINGLEAPPLICATION_BINARY_DIR "waitforprimaryexittest");
#else
    return QStringLiteral("waitforprimaryexittest/waitforprimaryexittest");
#endif
}

std::unique_ptr<QProcess> tst_WaitForPrimaryExitTest::startInstance(const QStringList &args, const QByteArray &expectedOutput)
{
    auto process = std::make_unique<QProcess>();
    process->setProcessChannelMode(QProcess::ForwardedErrorChannel);
    process->start(executable(), QStringList { m_testId } + args);
    if (!process->waitForStarted())
        return nullptr;

    QByteArray output;
    const bool ok = QTest::qWaitFor([&]() {
        output += process->readAllStandardOutput();
        return output == expectedOutput;
    });
    if (!ok)
        return nullptr;

    return process;
}

void tst_WaitForPrimaryExitTest::init()
{
    m_testId = QString::number(QRandomGenerator::global()->generate());
    m_primary = startInstance({ QStringLiteral("primary") }, "Primary\n");
    QVERIFY(m_primary);
}

void tst_WaitForPrimaryExitTest::cleanup()
{
    if (m_primary && m_primary->state() != QProcess::NotRunning) {
        // Shut the primary down cleanly, so it does not leave its socket/lock files behind
        auto quitter = startInstance({ QStringLiteral("quit") }, "Secondary\n");
        if (quitter)
            quitter->waitForFinished();
        if (!m_primary->waitForFinished()) {
            m_primary->kill();
            m_primary->waitForFinished();
        }
    }
    m_primary.reset();
}

void tst_WaitForPrimaryExitTest::testPrimaryQuits()
{
    auto waiter = startInstance({ QStringLiteral("wait") }, "Secondary\n");
    QVERIFY(waiter);

    // The waiter must keep blocking while the primary is alive
    QVERIFY(!waiter->waitForFinished(500));
    QCOMPARE(waiter->state(), QProcess::Running);

    auto quitter = startInstance({ QStringLiteral("quit") }, "Secondary\n");
    QVERIFY(quitter);
    if (quitter->state() != QProcess::NotRunning)
        QVERIFY(quitter->waitForFinished());
    QCOMPARE(quitter->exitCode(), 0);

    if (m_primary->state() != QProcess::NotRunning)
        QVERIFY(m_primary->waitForFinished());
    QCOMPARE(m_primary->exitCode(), 0);

    if (waiter->state() != QProcess::NotRunning)
        QVERIFY(waiter->waitForFinished());
    QCOMPARE(waiter->exitStatus(), QProcess::NormalExit);
    QCOMPARE(waiter->exitCode(), 0);
    QCOMPARE(waiter->readAllStandardOutput(), QByteArray("PrimaryExited\n"));
}

void tst_WaitForPrimaryExitTest::testPrimaryKilled()
{
    auto waiter = startInstance({ QStringLiteral("wait") }, "Secondary\n");
    QVERIFY(waiter);
    QVERIFY(!waiter->waitForFinished(500));

    // Simulate a crash: the lock file is left behind, but must be detected as stale
    m_primary->kill();
    QVERIFY(m_primary->waitForFinished());

    if (waiter->state() != QProcess::NotRunning)
        QVERIFY(waiter->waitForFinished());
    QCOMPARE(waiter->exitStatus(), QProcess::NormalExit);
    QCOMPARE(waiter->exitCode(), 0);
    QCOMPARE(waiter->readAllStandardOutput(), QByteArray("PrimaryExited\n"));

    // A new primary must be able to take over after the crash. This also removes
    // the stale socket; cleanup() then shuts it down cleanly.
    m_primary = startInstance({ QStringLiteral("primary") }, "Primary\n");
    QVERIFY(m_primary);
}

void tst_WaitForPrimaryExitTest::testTimeout()
{
    auto waiter = startInstance({ QStringLiteral("wait"), QStringLiteral("300") }, "Secondary\n");
    QVERIFY(waiter);

    if (waiter->state() != QProcess::NotRunning)
        QVERIFY(waiter->waitForFinished());
    QCOMPARE(waiter->exitStatus(), QProcess::NormalExit);
    QCOMPARE(waiter->exitCode(), 2);
    QCOMPARE(waiter->readAllStandardOutput(), QByteArray("Timeout\n"));

    // The primary is unaffected
    QCOMPARE(m_primary->state(), QProcess::Running);
}

QTEST_MAIN(tst_WaitForPrimaryExitTest)

#include "tst_waitforprimaryexittest.moc"
