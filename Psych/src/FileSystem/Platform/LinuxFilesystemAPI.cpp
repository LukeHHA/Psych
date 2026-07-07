#include "LinuxFilesystemAPI.h"

#include <cstdlib>

namespace psych
{
DirPath LinuxFilesystemAPI::GetOSAppDataPath() const
{
  if (const char* configHome = std::getenv("XDG_CONFIG_HOME")) {
    if (*configHome != '\0') {
      return DirPath{configHome};
    }
  }

  const auto homePath = GetHomePath();
  if (homePath.empty()) {
    return {};
  }

  return homePath / ".config";
}

DirPath LinuxFilesystemAPI::GetOSCacheDataPath() const
{
  if (const char* cacheHome = std::getenv("XDG_CACHE_HOME")) {
    if (*cacheHome != '\0') {
      return DirPath{cacheHome};
    }
  }

  const auto homePath = GetHomePath();
  if (homePath.empty()) {
    return {};
  }

  return homePath / ".cache";
}

DirPath LinuxFilesystemAPI::GetHomePath() const
{
  if (const char* home = std::getenv("HOME")) {
    if (*home != '\0') {
      return DirPath{home};
    }
  }

  return {};
}
} // namespace psych
