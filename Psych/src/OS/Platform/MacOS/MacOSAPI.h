#pragma once

#include "OS/OSAPI.h"
#include <filesystem>

namespace psych
{
class MacOSAPI : public OSAPI
{
public:
  MacOSAPI()                           = default;
  MacOSAPI(MacOSAPI&&)                 = default;
  MacOSAPI(const MacOSAPI&)            = default;
  MacOSAPI& operator=(MacOSAPI&&)      = default;
  MacOSAPI& operator=(const MacOSAPI&) = default;
  ~MacOSAPI() override                 = default;

  [[nodiscard]] std::filesystem::path GetExecPath() const override;
  [[nodiscard]] Expected<std::filesystem::path, errors::OSError> TryGetExecPath() const override;

private:
};
} // namespace psych
