#include <LingmoUtils/Process.h>

#include <QTest>

class tst_Process : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testRunCommand()
    {
        Lingmo::Process proc;
        auto result = proc.run(QStringLiteral("echo"), QStringList() << QStringLiteral("hello"));
        QVERIFY(result.success);
        QVERIFY(result.stdout.contains(QStringLiteral("hello")));
        QCOMPARE(result.exitCode, 0);
    }

    void testRunFailingCommand()
    {
        Lingmo::Process proc;
        auto result = proc.run(QStringLiteral("false"));
        QVERIFY(!result.success);
        QCOMPARE(result.exitCode, 1);
    }

    void testCommandExists()
    {
        QVERIFY(Lingmo::Process::commandExists(QStringLiteral("echo")));
        QVERIFY(!Lingmo::Process::commandExists(QStringLiteral("nonexistent_command_xyz")));
    }

    void testRunWithArguments()
    {
        Lingmo::Process proc;
        auto result = proc.run(QStringLiteral("ls"),
                               QStringList() << QStringLiteral("/"));
        QVERIFY(result.success);
    }

    void testRunInDir()
    {
        Lingmo::Process proc;
        auto result = proc.runInDir(QStringLiteral("/tmp"),
                                    QStringLiteral("pwd"));
        QVERIFY(result.success);
        QVERIFY(result.stdout.trimmed().contains(QStringLiteral("/tmp")));
    }

    void testTimeout()
    {
        Lingmo::Process proc;
        auto result = proc.run(QStringLiteral("sleep"), QStringList() << QStringLiteral("10"),
                               1000); // 1 second timeout
        QVERIFY(!result.success);
        QVERIFY(result.stderr.contains(QStringLiteral("timed out")));
    }
};

QTEST_MAIN(tst_Process)
#include "tst_Process.moc"
