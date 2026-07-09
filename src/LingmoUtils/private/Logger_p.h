#pragma once

#include <LingmoUtils/Logger.h>

#include <QMutex>
#include <QHash>

namespace Lingmo {

class LoggerPrivate
{
public:
    bool journaldEnabled = false;
    int minimumLevel = Logger::Info;
    QMutex mutex;
};

} // namespace Lingmo
