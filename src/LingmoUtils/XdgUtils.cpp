#include <LingmoUtils/XdgUtils.h>

#include <QDir>
#include <QStandardPaths>
#include <QProcessEnvironment>

namespace Lingmo {

QString XdgUtils::userConfigDir()
{
    const QString override = qEnvironmentVariable("LINGMO_CONFIG_DIR");
    if (!override.isEmpty()) return override;

    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
           + QStringLiteral("/lingmo");
}

QString XdgUtils::systemConfigDir()
{
    return QStringLiteral("/etc/lingmo");
}

QString XdgUtils::defaultConfigDir()
{
    return QStringLiteral("/usr/share/lingmo/defaults");
}

QString XdgUtils::userDataDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
           + QStringLiteral("/lingmo");
}

QString XdgUtils::systemDataDir()
{
    return QStringLiteral("/usr/share/lingmo");
}

QString XdgUtils::userCacheDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
           + QStringLiteral("/lingmo");
}

QString XdgUtils::userStateDir()
{
    const QString xdgState = qEnvironmentVariable("XDG_STATE_HOME");
    const QString base = xdgState.isEmpty()
        ? QDir::homePath() + QStringLiteral("/.local/state")
        : xdgState;
    return base + QStringLiteral("/lingmo");
}

QString XdgUtils::userRuntimeDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation)
           + QStringLiteral("/lingmo");
}

QStringList XdgUtils::iconDirs()
{
    QStringList dirs;

    // User icons
    const QString xdgData = qEnvironmentVariable("XDG_DATA_HOME");
    const QString userData = xdgData.isEmpty()
        ? QDir::homePath() + QStringLiteral("/.local/share")
        : xdgData;
    dirs << userData + QStringLiteral("/icons");

    // System icons
    dirs << QStringLiteral("/usr/share/icons");
    dirs << QStringLiteral("/usr/local/share/icons");

    return dirs;
}

QString XdgUtils::userAutostartDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
           + QStringLiteral("/autostart");
}

bool XdgUtils::ensureDir(const QString &path)
{
    return QDir().mkpath(path);
}

} // namespace Lingmo
