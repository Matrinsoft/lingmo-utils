#include <LingmoUtils/Logger.h>

#include <QTest>
#include <QSignalSpy>

class tst_Logger : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testInstance()
    {
        auto *logger = Lingmo::Logger::instance();
        QVERIFY(logger != nullptr);
        // Same instance on subsequent calls
        QCOMPARE(Lingmo::Logger::instance(), logger);
    }

    void testMinimumLevel()
    {
        auto *logger = Lingmo::Logger::instance();
        logger->setMinimumLevel(Lingmo::Logger::Warn);
        QCOMPARE(logger->minimumLevel(), static_cast<int>(Lingmo::Logger::Warn));

        logger->setMinimumLevel(Lingmo::Logger::Debug);
        QCOMPARE(logger->minimumLevel(), static_cast<int>(Lingmo::Logger::Debug));
    }

    void testLogEmitsSignal()
    {
        auto *logger = Lingmo::Logger::instance();
        logger->setMinimumLevel(Lingmo::Logger::Trace);

        QSignalSpy spy(logger, &Lingmo::Logger::messageLogged);
        logger->log("test.category", Lingmo::Logger::Info, QStringLiteral("test message"));
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toString(), QStringLiteral("test.category"));
        QCOMPARE(spy.at(0).at(1).toInt(), static_cast<int>(Lingmo::Logger::Info));
        QCOMPARE(spy.at(0).at(2).toString(), QStringLiteral("test message"));
    }

    void testMinimumLevelFiltering()
    {
        auto *logger = Lingmo::Logger::instance();
        logger->setMinimumLevel(Lingmo::Logger::Warn);

        QSignalSpy spy(logger, &Lingmo::Logger::messageLogged);
        logger->log("test", Lingmo::Logger::Debug, QStringLiteral("should be filtered"));
        QCOMPARE(spy.count(), 0);

        logger->log("test", Lingmo::Logger::Warn, QStringLiteral("should pass"));
        QCOMPARE(spy.count(), 1);
    }

    void testJournaldToggle()
    {
        auto *logger = Lingmo::Logger::instance();
        logger->setJournaldEnabled(true);
        QVERIFY(logger->isJournaldEnabled());
        logger->setJournaldEnabled(false);
        QVERIFY(!logger->isJournaldEnabled());
    }
};

QTEST_MAIN(tst_Logger)
#include "tst_Logger.moc"
