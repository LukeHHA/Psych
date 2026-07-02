#pragma once

#include "FileSystem/OSFilesystemAPI.h"

namespace ge
{
class LinuxFilesystemAPI : public OSFilesystemAPI
{
public:
  LinuxFilesystemAPI()           = default;
  ~LinuxFilesystemAPI() override = default;
  CORE_NO_COPY_NO_MOVE(LinuxFilesystemAPI);

  DirPath GetOSAppDataPath() const override;
  DirPath GetOSCacheDataPath() const override;

private:
  [[nodiscard]] DirPath GetHomePath() const;
};
} // namespace ge
