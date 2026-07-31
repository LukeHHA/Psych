#pragma once

#include "Core/Core.h"
#include "Errors/Errors.h"
#include "OS/OSAPI.h"
#include "expected.h"
#include <filesystem>

namespace psych
{
class OS
{
public:
  OS()                     = default;
  OS(OS&&)                 = default;
  OS(const OS&)            = delete;
  OS& operator=(OS&&)      = default;
  OS& operator=(const OS&) = delete;
  virtual ~OS()            = default;

  static Expected<void, errors::OSError> Init();
  static Expected<void, errors::OSError> Shutdown();

  [[nodiscard]] static std::filesystem::path GetExecPath();
  [[nodiscard]] static Expected<std::filesystem::path, errors::OSError> TryGetExecPath();

private:
  inline static Unique<OSAPI> s_OSAPI_ = nullptr;
};
} // namespace psych
