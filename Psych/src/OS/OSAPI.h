#pragma once

#include "Core/Core.h"
#include "Errors/Errors.h"
#include "expected.h"
#include <filesystem>

namespace psych
{
class OSAPI
{
public:
  OSAPI()                                                                                       = default;
  OSAPI(OSAPI&&)                                                                                = default;
  OSAPI(const OSAPI&)                                                                           = default;
  OSAPI& operator=(OSAPI&&)                                                                     = default;
  OSAPI& operator=(const OSAPI&)                                                                = default;
  virtual ~OSAPI()                                                                              = default;

  [[nodiscard]] virtual std::filesystem::path GetExecPath() const                               = 0;
  [[nodiscard]] virtual Expected<std::filesystem::path, errors::OSError> TryGetExecPath() const = 0;

  static Unique<OSAPI> Create();

private:
};
} // namespace psych
