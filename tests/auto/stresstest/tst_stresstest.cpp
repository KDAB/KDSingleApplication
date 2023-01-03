/*
  This file is part of KDSingleApplication.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include <QtCore/QProcess>
#include <QtCore/QRandomGenerator>
#include <QtTest/QTest>

#include <vector>
#include <memory>

#ifdef Q_OS_UNIX
#include <sys/time.h>
#include <sys/resource.h>
#endif

class tst_StressTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testOnePrimaryManySecondaries();

    void testOnlyOnePrimary_data();
    void testOnlyOnePrimary();
};

// The default timeouts don't seem to be enough
// on Windows/OSX; increase it when the number of instances grows.
static int calculateTimeout(int instances)
{
    return qMax(instances * 5000, 30000);
}

void tst_StressTest::initTestCase()
{
#ifdef Q_OS_UNIX
    // increase the system limits if possible
    struct rlimit ulimit;
    if (::getrlimit(RLIMIT_NOFILE, &ulimit) != 0) {
        perror("Cannot get ulimits");
        QFAIL("Aborting testcase");
    }

    // up to 100 processes, up to 4 fds per process;
    // this should be more than enough!
    constexpr int minimumNumberOfFds = 4096;

    if (ulimit.rlim_cur < minimumNumberOfFds) {
        ulimit.rlim_cur = minimumNumberOfFds;

        if (::setrlimit(RLIMIT_NOFILE, &ulimit) != 0) {
            perror("Cannot set ulimits");
            QFAIL("Aborting testcase");
        }
    }
#endif
}

void tst_StressTest::testOnePrimaryManySecondaries()
{
    // Each secondary will output runsPerSecondary _blocks_ of messages;
    // we run secondariesCount secondaries.
    // * The primary gets: how many blocks of messages to expect in total
    // * The secondaries get: how many blocks to send
    //
    // (The primary will do the math internally)

#ifdef KDSINGLEAPPLICATION_BINARY_DIR
    const QString executable = QStringLiteral(KDSINGLEAPPLICATION_BINARY_DIR "stresstest");
#else
    const QString executable = QStringLiteral("stresstest/stresstest");
#endif

    const int secondariesCount = 50;
    const int runsPerSecondary = 50;
    const QString testId = QString::number(QRandomGenerator::global()->generate());
    const int timeout = calculateTimeout(secondariesCount);
    const QString timeoutString = QString::number(timeout);

    QByteArray output;
    bool ok;

    QProcess primary;
    primary.setProcessChannelMode(QProcess::ForwardedErrorChannel);
    primary.start(executable, { testId, QStringLiteral("primary"), timeoutString, QString::number(secondariesCount * runsPerSecondary) });
    QVERIFY(primary.waitForStarted());
    QCOMPARE(primary.state(), QProcess::Running);
    output.clear();
    ok = QTest::qWaitFor([&]() {
        output += primary.readAllStandardOutput();
        return output == "Primary\n";
    });
    QVERIFY(ok);

    std::vector<std::unique_ptr<QProcess>> secondaries;
    secondaries.reserve(secondariesCount);
    for (int i = 0; i < secondariesCount; ++i) {
        std::unique_ptr<QProcess> secondary = std::make_unique<QProcess>();
        secondary->setProcessChannelMode(QProcess::ForwardedErrorChannel);
        secondary->start(executable, { testId, QStringLiteral("secondary"), timeoutString, QString::number(runsPerSecondary) });
        secondaries.push_back(std::move(secondary));
    }

    for (auto &secondary : secondaries)
        QVERIFY(secondary->waitForStarted(timeout));

    for (auto &secondary : secondaries) {
        if (secondary->state() != QProcess::NotRunning)
            QVERIFY(secondary->waitForFinished(timeout));
        QCOMPARE(secondary->exitCode(), 0);
        QCOMPARE(secondary->exitStatus(), QProcess::NormalExit);
    }

    secondaries.clear();

    if (primary.state() != QProcess::NotRunning)
        QVERIFY(primary.waitForFinished(timeout));
    QCOMPARE(primary.exitCode(), 0);
    QCOMPARE(primary.exitStatus(), QProcess::NormalExit);
}

void tst_StressTest::testOnlyOnePrimary_data()
{
    QTest::addColumn<int>("count");

    for (int i = 10; i <= 100; i += 10)
        QTest::addRow("count-%d", i) << i;
}

void tst_StressTest::testOnlyOnePrimary()
{
    QFETCH(int, count);

    int primaryCount = 0;
    int secondaryCount = 0;

#ifdef KDSINGLEAPPLICATION_BINARY_DIR
    const QString executable = QStringLiteral(KDSINGLEAPPLICATION_BINARY_DIR "stresstest2");
#else
    const QString executable = QStringLiteral("stresstest2/stresstest2");
#endif

    const QString countString = QString::number(count);
    const QString testId = QString::number(QRandomGenerator::global()->generate());

    const auto timeout = calculateTimeout(count);
    const auto timeoutString = QString::number(timeout);

    std::vector<std::unique_ptr<QProcess>> processes;
    processes.reserve(count);
    for (int i = 0; i < count; ++i) {
        std::unique_ptr<QProcess> process = std::make_unique<QProcess>();
        process->setProcessChannelMode(QProcess::ForwardedErrorChannel);
        process->start(executable, { testId, timeoutString, countString });
        processes.push_back(std::move(process));
    }

    for (auto &process : processes) {
        if (process->state() != QProcess::NotRunning)
            QVERIFY(process->waitForFinished(timeout));
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

QTEST_MAIN(tst_StressTest)

#include "tst_stresstest.moc"
