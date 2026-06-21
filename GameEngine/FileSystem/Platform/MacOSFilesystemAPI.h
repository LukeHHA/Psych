#include "FileSystem/OSFilesystemAPI.h"
namespace ge
{
class MacOSFilesystemAPI : public OSFilesystemAPI
{

public:
  MacOSFilesystemAPI()                                     = default;
  MacOSFilesystemAPI(MacOSFilesystemAPI&&)                 = default;
  MacOSFilesystemAPI(const MacOSFilesystemAPI&)            = default;
  MacOSFilesystemAPI& operator=(MacOSFilesystemAPI&&)      = default;
  MacOSFilesystemAPI& operator=(const MacOSFilesystemAPI&) = default;
  ~MacOSFilesystemAPI()                                    = default;

  virtual DirPath GetOSAppDataPath() const override;
  virtual DirPath GetOSCacheDataPath() const override;

private:
  std::filesystem::path GetApplicationSupportPath() const;
  std::filesystem::path GetCachePath() const;
};
} // namespace ge
