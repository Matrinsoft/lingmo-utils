#include "private/FileUtils_p.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDirIterator>
#include <QMimeDatabase>
#include <QTemporaryFile>

namespace Lingmo {

FileUtils::FileUtils(QObject *parent)
    : QObject(parent)
    , d(std::make_unique<FileUtilsPrivate>())
{
}

FileUtils::~FileUtils() = default;

bool FileUtils::copy(const QString &source, const QString &destination,
                      ConflictAction conflict)
{
    const QFileInfo srcInfo(source);
    if (!srcInfo.exists()) {
        emit error(source, QStringLiteral("Source does not exist"));
        return false;
    }

    if (srcInfo.isDir()) {
        QDir destDir(destination);
        if (!destDir.exists() && !destDir.mkpath(QStringLiteral("."))) {
            emit error(destination, QStringLiteral("Cannot create directory"));
            return false;
        }

        QDirIterator it(source, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot,
                        QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            const QString relPath = QDir(source).relativeFilePath(it.filePath());
            const QString destPath = destination + QLatin1Char('/') + relPath;

            if (it.fileInfo().isDir()) {
                QDir().mkpath(destPath);
            } else {
                if (QFileInfo::exists(destPath)) {
                    if (conflict == Skip) continue;
                    if (conflict == Overwrite) {
                        QFile::remove(destPath);
                    } else if (conflict == Rename) {
                        // Generate unique name
                        int n = 1;
                        QString newPath = destPath;
                        while (QFileInfo::exists(newPath)) {
                            newPath = destPath + QStringLiteral("~%1").arg(n++);
                        }
                        // Will copy to newPath below
                    }
                }
                if (!QFile::copy(it.filePath(), destPath)) {
                    emit error(it.filePath(), QStringLiteral("Copy failed"));
                    return false;
                }
            }
        }
        return true;
    }

    // Single file copy
    if (QFileInfo::exists(destination)) {
        if (conflict == Skip) return true;
        if (conflict == Overwrite) {
            QFile::remove(destination);
        }
    }

    if (!QFile::copy(source, destination)) {
        emit error(source, QStringLiteral("Copy failed"));
        return false;
    }
    return true;
}

bool FileUtils::move(const QString &source, const QString &destination,
                      ConflictAction conflict)
{
    if (!QFileInfo::exists(source)) {
        emit error(source, QStringLiteral("Source does not exist"));
        return false;
    }

    if (QFileInfo::exists(destination)) {
        if (conflict == Skip) return true;
        if (conflict == Overwrite) {
            if (QFileInfo(destination).isDir()) {
                QDir(destination).removeRecursively();
            } else {
                QFile::remove(destination);
            }
        }
    }

    if (!QFile::rename(source, destination)) {
        // Cross-device move: copy + delete
        if (copy(source, destination, conflict)) {
            return remove(source);
        }
        emit error(source, QStringLiteral("Move failed"));
        return false;
    }
    return true;
}

bool FileUtils::remove(const QString &path)
{
    const QFileInfo info(path);
    if (!info.exists()) return true;

    if (info.isDir()) {
        return QDir(path).removeRecursively();
    }
    return QFile::remove(path);
}

bool FileUtils::mkdir(const QString &path)
{
    return QDir().mkpath(path);
}

bool FileUtils::writeAtomic(const QString &filePath, const QByteArray &content)
{
    QFileInfo fi(filePath);
    if (!fi.dir().exists()) {
        QDir().mkpath(fi.dir().path());
    }

    QTemporaryFile tmp(fi.dir().filePath(QStringLiteral(".tmp-XXXXXX")));
    if (!tmp.open())
        return false;

    tmp.setAutoRemove(false);
    if (tmp.write(content) != content.size()) {
        tmp.remove();
        return false;
    }

    tmp.close();

    // Atomic rename
    if (QFileInfo::exists(filePath))
        QFile::remove(filePath);

    return tmp.rename(filePath);
}

bool FileUtils::isReadable(const QString &path) const
{
    return QFileInfo(path).isReadable();
}

bool FileUtils::isWritable(const QString &path) const
{
    return QFileInfo(path).isWritable();
}

QString FileUtils::mimeType(const QString &filePath) const
{
    QMimeDatabase db;
    return db.mimeTypeForFile(filePath).name();
}

QString FileUtils::formatFileSize(qint64 bytes)
{
    if (bytes < 1024)
        return QStringLiteral("%1 B").arg(bytes);
    if (bytes < 1024 * 1024)
        return QStringLiteral("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    if (bytes < 1024LL * 1024 * 1024)
        return QStringLiteral("%1 MB").arg(bytes / (1024.0 * 1024), 0, 'f', 1);
    return QStringLiteral("%1 GB").arg(bytes / (1024.0 * 1024 * 1024), 0, 'f', 1);
}

void FileUtils::setProgressCallback(ProgressCallback callback)
{
    d->progressCallback = std::move(callback);
}

} // namespace Lingmo
