#pragma once

#include "FileSystem/OSFilesystemAPI.h"

namespace psych
{
class MacOSFilesystemAPI : public OSFilesystemAPI
{

public:
  MacOSFilesystemAPI()           = default;
  ~MacOSFilesystemAPI() override = default;
  CORE_NO_COPY_NO_MOVE(MacOSFilesystemAPI);

  DirPath GetOSAppDataPath() const override;
  DirPath GetOSCacheDataPath() const override;

private:
  std::filesystem::path GetApplicationSupportPath() const;
  std::filesystem::path GetCachePath() const;
};
} // namespace psych
