#pragma once

#include <LingmoUtils/FileUtils.h>

#include <QDir>

namespace Lingmo {

class FileUtilsPrivate
{
public:
    FileUtils::ProgressCallback progressCallback;
};

} // namespace Lingmo
