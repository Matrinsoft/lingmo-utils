#include <LingmoUtils/XdgUtils.h>

#include <QTest>
#include <QDir>
#include <QStandardPaths>

class tst_XdgUtils : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testUserConfigDir()
    {
        const QString dir = Lingmo::XdgUtils::userConfigDir();
        QVERIFY(!dir.isEmpty());
        QVERIFY(dir.endsWith(QStringLiteral("/lingmo")));
    }

    void testSystemConfigDir()
    {
        QCOMPARE(Lingmo::XdgUtils::systemConfigDir(),
                 QStringLiteral("/etc/lingmo"));
    }

    void testDefaultConfigDir()
    {
        QCOMPARE(Lingmo::XdgUtils::defaultConfigDir(),
                 QStringLiteral("/usr/share/lingmo/defaults"));
    }

    void testUserDataDir()
    {
        const QString dir = Lingmo::XdgUtils::userDataDir();
        QVERIFY(!dir.isEmpty());
        QVERIFY(dir.endsWith(QStringLiteral("/lingmo")));
    }

    void testSystemDataDir()
    {
        QCOMPARE(Lingmo::XdgUtils::systemDataDir(),
                 QStringLiteral("/usr/share/lingmo"));
    }

    void testUserCacheDir()
    {
        const QString dir = Lingmo::XdgUtils::userCacheDir();
        QVERIFY(!dir.isEmpty());
    }

    void testIconDirs()
    {
        const QStringList dirs = Lingmo::XdgUtils::iconDirs();
        QVERIFY(!dirs.isEmpty());
        QVERIFY(dirs.contains(QStringLiteral("/usr/share/icons")));
    }

    void testUserAutostartDir()
    {
        const QString dir = Lingmo::XdgUtils::userAutostartDir();
        QVERIFY(dir.endsWith(QStringLiteral("/autostart")));
    }

    void testEnsureDir()
    {
        QTemporaryDir tmpDir;
        QVERIFY(tmpDir.isValid());

        const QString newDir = tmpDir.filePath(QStringLiteral("a/b/c"));
        QVERIFY(Lingmo::XdgUtils::ensureDir(newDir));
        QVERIFY(QDir(newDir).exists());
    }

    void testEnvOverride()
    {
        // Set override
        qputenv("LINGMO_CONFIG_DIR", "/tmp/test-override");
        const QString dir = Lingmo::XdgUtils::userConfigDir();
        QCOMPARE(dir, QStringLiteral("/tmp/test-override"));
        qunsetenv("LINGMO_CONFIG_DIR");

        // After unsetting, should use default
        const QString defaultDir = Lingmo::XdgUtils::userConfigDir();
        QVERIFY(defaultDir.endsWith(QStringLiteral("/lingmo")));
    }
};

QTEST_MAIN(tst_XdgUtils)
#include "tst_XdgUtils.moc"
