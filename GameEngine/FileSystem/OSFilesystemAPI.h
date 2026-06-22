#pragma once

#include "Core/Core.h"
#include <filesystem>

namespace ge
{
using FilePath = std::filesystem::path;
using DirPath  = std::filesystem::path;

using recursive_directory_iterator =
    std::filesystem::recursive_directory_iterator;

class OSFilesystemAPI
{
public:
  OSFilesystemAPI()          = default;
  virtual ~OSFilesystemAPI() = default;
  CORE_NO_COPY_NO_MOVE(OSFilesystemAPI);

  virtual DirPath GetOSAppDataPath() const   = 0;
  virtual DirPath GetOSCacheDataPath() const = 0;
};
} // namespace ge
