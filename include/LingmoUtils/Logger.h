#pragma once

#include <LingmoUtils/LingmoUtilsExport.h>

#include <QObject>
#include <QString>
#include <QLoggingCategory>
#include <memory>

namespace Lingmo {

class LoggerPrivate;

// Structured logging for Lingmo Desktop components.
//
// Usage:
//   LINGMO_LOG_INFO("lingmo.theme") << "Theme loaded:" << name;
//   LINGMO_LOG_WARN("lingmo.config") << "Key not found:" << key;
//
// Log levels: Trace, Debug, Info, Warn, Error, Fatal
// Output: stderr by default, journald when available
class LINGMOUTILS_EXPORT Logger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool journaldEnabled READ isJournaldEnabled WRITE setJournaldEnabled)
    Q_PROPERTY(int minimumLevel READ minimumLevel WRITE setMinimumLevel)

public:
    enum Level {
        Trace   = 0,
        Debug   = 1,
        Info    = 2,
        Warn    = 3,
        Error   = 4,
        Fatal   = 5,
        Off     = 6
    };
    Q_ENUM(Level)

    explicit Logger(QObject *parent = nullptr);
    ~Logger() override;

    static Logger *instance();

    bool isJournaldEnabled() const;
    void setJournaldEnabled(bool enabled);

    int minimumLevel() const;
    void setMinimumLevel(int level);

    // Log a message with the given category and level
    void log(const char *category, int level, const QString &message);

    // Install as the Qt message handler
    void install();

Q_SIGNALS:
    void messageLogged(const QString &category, int level, const QString &message);

private:
    std::unique_ptr<LoggerPrivate> d;
};

} // namespace Lingmo

// Convenience macros
#define LINGMO_LOG_TRACE(cat) \
    if (qCDebug(LingmoLog_##cat).isDebugEnabled()) \
        QMessageLogger(__FILE__, __LINE__, #cat).debug()
#define LINGMO_LOG_DEBUG(cat) \
    QMessageLogger(__FILE__, __LINE__, #cat).debug()
#define LINGMO_LOG_INFO(cat) \
    QMessageLogger(__FILE__, __LINE__, #cat).info()
#define LINGMO_LOG_WARN(cat) \
    QMessageLogger(__FILE__, __LINE__, #cat).warning()
#define LINGMO_LOG_ERROR(cat) \
    QMessageLogger(__FILE__, __LINE__, #cat).critical()

// Declare a logging category
#define LINGMO_DECLARE_LOGGING_CATEGORY(name) \
    Q_DECLARE_LOGGING_CATEGORY(name)

// Define a logging category
#define LINGMO_DEFINE_LOGGING_CATEGORY(name, catString) \
    Q_LOGGING_CATEGORY(name, catString)
