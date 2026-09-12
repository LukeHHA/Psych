
#pragma once

#include "OS/OSAPI.h"
#include <filesystem>

namespace psych
{
class LinuxOSAPI : public OSAPI
{
public:
  LinuxOSAPI()                             = default;
  LinuxOSAPI(LinuxOSAPI&&)                 = default;
  LinuxOSAPI(const LinuxOSAPI&)            = default;
  LinuxOSAPI& operator=(LinuxOSAPI&&)      = default;
  LinuxOSAPI& operator=(const LinuxOSAPI&) = default;
  ~LinuxOSAPI() override                   = default;

  [[nodiscard]] std::filesystem::path GetExecPath() const override;
  [[nodiscard]] Expected<std::filesystem::path, errors::OSError> TryGetExecPath() const override;

private:
};
} // namespace psych
