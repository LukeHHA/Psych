#include "MacOSFilesystemAPI.h"
#include "FileSystem/OSFilesystemAPI.h"

namespace ge
{
DirPath MacOSFilesystemAPI::GetOSAppDataPath() const
{
  return GetApplicationSupportPath();
}

DirPath MacOSFilesystemAPI::GetOSCacheDataPath() const
{
  return GetCachePath();
}

} // namespace ge
