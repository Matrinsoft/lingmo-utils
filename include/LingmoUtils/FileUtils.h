#pragma once

#include <LingmoUtils/LingmoUtilsExport.h>

#include <QObject>
#include <QString>
#include <QFileInfo>
#include <functional>
#include <memory>

namespace Lingmo {

class FileUtilsPrivate;

// File operation utilities with progress reporting.
//
// Provides safe file operations:
// - Copy/move/delete with progress callbacks
// - Recursive directory operations
// - Conflict resolution strategies
// - Atomic write (write to temp, then rename)
class LINGMOUTILS_EXPORT FileUtils : public QObject
{
    Q_OBJECT

public:
    enum ConflictAction {
        Skip,
        Overwrite,
        Rename,
        Ask
    };
    Q_ENUM(ConflictAction)

    explicit FileUtils(QObject *parent = nullptr);
    ~FileUtils() override;

    // Copy a file or directory
    bool copy(const QString &source, const QString &destination,
              ConflictAction conflict = Skip);

    // Move/rename a file or directory
    bool move(const QString &source, const QString &destination,
              ConflictAction conflict = Skip);

    // Remove a file or directory recursively
    bool remove(const QString &path);

    // Create directory recursively (like mkdir -p)
    bool mkdir(const QString &path);

    // Atomic write: write content to a temp file, then rename
    bool writeAtomic(const QString &filePath, const QByteArray &content);

    // Check if a path is readable/writable
    bool isReadable(const QString &path) const;
    bool isWritable(const QString &path) const;

    // Get MIME type of a file
    QString mimeType(const QString &filePath) const;

    // File size formatting (e.g., "1.2 GB")
    static QString formatFileSize(qint64 bytes);

    // Set progress callback (called during copy/move operations)
    using ProgressCallback = std::function<bool(qint64 current, qint64 total)>;
    void setProgressCallback(ProgressCallback callback);

Q_SIGNALS:
    void error(const QString &path, const QString &message);

private:
    std::unique_ptr<FileUtilsPrivate> d;
};

} // namespace Lingmo
