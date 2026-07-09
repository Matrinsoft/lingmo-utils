#pragma once

#include <LingmoUtils/LingmoUtilsExport.h>

#include <QString>
#include <QStringList>

namespace Lingmo {

// XDG Base Directory Specification utilities.
//
// Resolves standard paths according to the XDG Base Directory spec:
//   XDG_CONFIG_HOME   ~/.config
//   XDG_DATA_HOME     ~/.local/share
//   XDG_CACHE_HOME    ~/.cache
//   XDG_STATE_HOME    ~/.local/state
//   XDG_RUNTIME_DIR   /run/user/$UID
//
// All paths respect the LINGMO_ override convention:
//   LINGMO_CONFIG_DIR overrides XDG_CONFIG_HOME for Lingmo components.
class LINGMOUTILS_EXPORT XdgUtils
{
public:
    // ── Config paths ──────────────────────────────────────

    // User config directory (~/.config/lingmo/)
    static QString userConfigDir();

    // System config directory (/etc/lingmo/)
    static QString systemConfigDir();

    // Default config directory (/usr/share/lingmo/defaults/)
    static QString defaultConfigDir();

    // ── Data paths ────────────────────────────────────────

    // User data directory (~/.local/share/lingmo/)
    static QString userDataDir();

    // System data directory (/usr/share/lingmo/)
    static QString systemDataDir();

    // ── Cache paths ───────────────────────────────────────

    // User cache directory (~/.cache/lingmo/)
    static QString userCacheDir();

    // ── State paths ───────────────────────────────────────

    // User state directory (~/.local/state/lingmo/)
    static QString userStateDir();

    // ── Runtime paths ─────────────────────────────────────

    // User runtime directory (/run/user/$UID/lingmo/)
    static QString userRuntimeDir();

    // ── Icon paths ────────────────────────────────────────

    // System icon directories
    static QStringList iconDirs();

    // ── Autostart ─────────────────────────────────────────

    // User autostart directory (~/.config/autostart/)
    static QString userAutostartDir();

    // ── Helpers ───────────────────────────────────────────

    // Ensure a directory exists (create if not)
    static bool ensureDir(const QString &path);

private:
    XdgUtils() = delete;
};

} // namespace Lingmo
