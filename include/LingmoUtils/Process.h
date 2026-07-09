#pragma once

#include <LingmoUtils/LingmoUtilsExport.h>

#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <memory>

namespace Lingmo {

class ProcessPrivate;

// Process spawning and management utilities.
//
// Provides:
// - Synchronous command execution with output capture
// - Async process management
// - Environment variable control
// - Exit code handling
class LINGMOUTILS_EXPORT Process : public QObject
{
    Q_OBJECT

public:
    struct Result {
        int exitCode = -1;
        QString stdout;
        QString stderr;
        bool success = false;
    };

    explicit Process(QObject *parent = nullptr);
    ~Process() override;

    // Run a command synchronously and capture output
    Result run(const QString &program,
               const QStringList &arguments = {},
               int timeoutMs = 30000);

    // Run with working directory
    Result runInDir(const QString &workingDir,
                    const QString &program,
                    const QStringList &arguments = {},
                    int timeoutMs = 30000);

    // Check if a command exists in PATH
    static bool commandExists(const QString &command);

    // Get the full path of a command
    static QString commandPath(const QString &command);

Q_SIGNALS:
    void finished(int exitCode);
    void errorOccurred(const QString &message);

private:
    std::unique_ptr<ProcessPrivate> d;
};

} // namespace Lingmo
