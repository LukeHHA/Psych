#pragma once

#include "Core/Base.h"
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
  OSFilesystemAPI()  = default;
  ~OSFilesystemAPI() = default;
  CORE_DELETE_COPY_OPS(OSFilesystemAPI);

  virtual DirPath GetOSAppDataPath() const   = 0;
  virtual DirPath GetOSCacheDataPath() const = 0;
};
} // namespace ge
