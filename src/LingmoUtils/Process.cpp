#include "private/Process_p.h"

#include <QProcess>
#include <QDir>
#include <QStandardPaths>

namespace Lingmo {

Process::Process(QObject *parent)
    : QObject(parent)
    , d(std::make_unique<ProcessPrivate>())
{
}

Process::~Process() = default;

Process::Result Process::run(const QString &program,
                              const QStringList &arguments,
                              int timeoutMs)
{
    return runInDir(QString(), program, arguments, timeoutMs);
}

Process::Result Process::runInDir(const QString &workingDir,
                                   const QString &program,
                                   const QStringList &arguments,
                                   int timeoutMs)
{
    Result result;

    QProcess process;
    if (!workingDir.isEmpty())
        process.setWorkingDirectory(workingDir);

    process.start(program, arguments);

    if (!process.waitForStarted(5000)) {
        result.stderr = QStringLiteral("Failed to start: %1").arg(program);
        emit errorOccurred(result.stderr);
        return result;
    }

    if (!process.waitForFinished(timeoutMs)) {
        process.kill();
        process.waitForFinished(1000);
        result.stderr = QStringLiteral("Process timed out: %1").arg(program);
        emit errorOccurred(result.stderr);
        return result;
    }

    result.exitCode = process.exitCode();
    result.stdout = QString::fromUtf8(process.readAllStandardOutput());
    result.stderr = QString::fromUtf8(process.readAllStandardError());
    result.success = (result.exitCode == 0);

    emit finished(result.exitCode);
    return result;
}

bool Process::commandExists(const QString &command)
{
    return !commandPath(command).isEmpty();
}

QString Process::commandPath(const QString &command)
{
    // Check if it's an absolute path
    if (QFileInfo(command).isAbsolute()) {
        return QFileInfo(command).exists() ? command : QString();
    }

    // Search in PATH
    const QStringList paths = QProcessEnvironment::systemEnvironment()
        .value(QStringLiteral("PATH"))
        .split(QLatin1Char(':'));

    for (const QString &dir : paths) {
        const QString fullPath = dir + QLatin1Char('/') + command;
        if (QFileInfo(fullPath).isExecutable())
            return fullPath;
    }

    return QString();
}

} // namespace Lingmo
