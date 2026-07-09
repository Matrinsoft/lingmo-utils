#include "private/Logger_p.h"

#include <QDebug>
#include <QCoreApplication>
#include <QMutexLocker>

namespace Lingmo {

static Logger *s_instance = nullptr;

// Custom Qt message handler that routes through Logger
static void lingmoMessageHandler(QtMsgType type, const QMessageLogContext &context,
                                  const QString &message)
{
    if (!s_instance) {
        // Fallback: just output to stderr
        fprintf(stderr, "%s\n", qPrintable(message));
        return;
    }

    int level;
    switch (type) {
    case QtDebugMsg:    level = Logger::Debug; break;
    case QtInfoMsg:     level = Logger::Info; break;
    case QtWarningMsg:  level = Logger::Warn; break;
    case QtCriticalMsg: level = Logger::Error; break;
    case QtFatalMsg:    level = Logger::Fatal; break;
    default:            level = Logger::Debug; break;
    }

    const char *category = context.category ? context.category : "default";
    s_instance->log(category, level, message);

    // Always abort on fatal
    if (type == QtFatalMsg)
        abort();
}

// ── Logger ───────────────────────────────────────────────

Logger::Logger(QObject *parent)
    : QObject(parent)
    , d(std::make_unique<LoggerPrivate>())
{
    Q_ASSERT(!s_instance);
    s_instance = this;
}

Logger::~Logger()
{
    s_instance = nullptr;
}

Logger *Logger::instance()
{
    if (!s_instance) {
        // Create a static instance that lives for the process lifetime
        static Logger logger;
        s_instance = &logger;
    }
    return s_instance;
}

bool Logger::isJournaldEnabled() const
{
    return d->journaldEnabled;
}

void Logger::setJournaldEnabled(bool enabled)
{
    d->journaldEnabled = enabled;
}

int Logger::minimumLevel() const
{
    return d->minimumLevel;
}

void Logger::setMinimumLevel(int level)
{
    d->minimumLevel = level;
}

void Logger::log(const char *category, int level, const QString &message)
{
    if (level < d->minimumLevel)
        return;

    QMutexLocker locker(&d->mutex);

    // Format: [LEVEL] category: message
    static const char *levelNames[] = {
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "OFF"
    };
    const char *levelStr = (level >= 0 && level <= 6) ? levelNames[level] : "???";

    const QString formatted = QStringLiteral("[%1] %2: %3")
        .arg(QString::fromLatin1(levelStr))
        .arg(QString::fromLatin1(category))
        .arg(message);

    // Output to stderr
    if (level >= Logger::Warn) {
        fprintf(stderr, "%s\n", qPrintable(formatted));
    } else {
        fprintf(stdout, "%s\n", qPrintable(formatted));
    }

    emit messageLogged(QString::fromLatin1(category), level, message);
}

void Logger::install()
{
    qInstallMessageHandler(lingmoMessageHandler);
}

} // namespace Lingmo
