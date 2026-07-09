#include <LingmoUtils/FileUtils.h>

#include <QTest>
#include <QTemporaryDir>
#include <QFile>
#include <QTextStream>

class tst_FileUtils : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testCopyFile()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        // Create source file
        const QString src = dir.filePath(QStringLiteral("source.txt"));
        QFile srcFile(src);
        QVERIFY(srcFile.open(QIODevice::WriteOnly | QIODevice::Text));
        QTextStream(&srcFile) << QStringLiteral("hello");
        srcFile.close();

        // Copy
        Lingmo::FileUtils fu;
        const QString dst = dir.filePath(QStringLiteral("dest.txt"));
        QVERIFY(fu.copy(src, dst));
        QVERIFY(QFileInfo::exists(dst));

        // Verify content
        QFile dstFile(dst);
        QVERIFY(dstFile.open(QIODevice::ReadOnly | QIODevice::Text));
        QCOMPARE(dstFile.readAll(), QByteArray("hello"));
    }

    void testMoveFile()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        const QString src = dir.filePath(QStringLiteral("source.txt"));
        QFile(src).open(QIODevice::WriteOnly);
        QFile(src).close();

        Lingmo::FileUtils fu;
        const QString dst = dir.filePath(QStringLiteral("moved.txt"));
        QVERIFY(fu.move(src, dst));
        QVERIFY(!QFileInfo::exists(src));
        QVERIFY(QFileInfo::exists(dst));
    }

    void testRemoveFile()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        const QString path = dir.filePath(QStringLiteral("to_delete.txt"));
        QFile(path).open(QIODevice::WriteOnly);
        QFile(path).close();

        Lingmo::FileUtils fu;
        QVERIFY(fu.remove(path));
        QVERIFY(!QFileInfo::exists(path));
    }

    void testMkdir()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        Lingmo::FileUtils fu;
        const QString nested = dir.filePath(QStringLiteral("a/b/c/d"));
        QVERIFY(fu.mkdir(nested));
        QVERIFY(QDir(nested).exists());
    }

    void testWriteAtomic()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        Lingmo::FileUtils fu;
        const QString path = dir.filePath(QStringLiteral("atomic.txt"));
        QVERIFY(fu.writeAtomic(path, QByteArray("atomic content")));

        QFile file(path);
        QVERIFY(file.open(QIODevice::ReadOnly));
        QCOMPARE(file.readAll(), QByteArray("atomic content"));
    }

    void testMimeType()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        const QString path = dir.filePath(QStringLiteral("test.txt"));
        QFile file(path);
        QVERIFY(file.open(QIODevice::WriteOnly));
        file.write("text");
        file.close();

        Lingmo::FileUtils fu;
        const QString mime = fu.mimeType(path);
        QVERIFY(mime.contains(QStringLiteral("text/plain")));
    }

    void testFormatFileSize()
    {
        QCOMPARE(Lingmo::FileUtils::formatFileSize(0), QStringLiteral("0 B"));
        QCOMPARE(Lingmo::FileUtils::formatFileSize(512), QStringLiteral("512 B"));
        QCOMPARE(Lingmo::FileUtils::formatFileSize(1024), QStringLiteral("1.0 KB"));
        QCOMPARE(Lingmo::FileUtils::formatFileSize(1024 * 1024), QStringLiteral("1.0 MB"));
    }

    void testCopyDirectory()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());

        // Create source directory with files
        const QString srcDir = dir.filePath(QStringLiteral("srcdir"));
        QDir(srcDir).mkpath(QStringLiteral("."));
        QFile(srcDir + QStringLiteral("/a.txt")).open(QIODevice::WriteOnly);
        QFile(srcDir + QStringLiteral("/a.txt")).close();
        QDir(srcDir).mkpath(QStringLiteral("sub"));
        QFile(srcDir + QStringLiteral("/sub/b.txt")).open(QIODevice::WriteOnly);
        QFile(srcDir + QStringLiteral("/sub/b.txt")).close();

        Lingmo::FileUtils fu;
        const QString dstDir = dir.filePath(QStringLiteral("dstdir"));
        QVERIFY(fu.copy(srcDir, dstDir));
        QVERIFY(QFileInfo::exists(dstDir + QStringLiteral("/a.txt")));
        QVERIFY(QFileInfo::exists(dstDir + QStringLiteral("/sub/b.txt")));
    }
};

QTEST_MAIN(tst_FileUtils)
#include "tst_FileUtils.moc"
