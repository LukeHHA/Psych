#include "LinuxOSAPI.h"
#include "Debug/Assert.h"
#include "Errors/Errors.h"
#include <filesystem>
#include <system_error>

namespace psych
{
std::filesystem::path LinuxOSAPI::GetExecPath() const
{
  auto result = TryGetExecPath();
  CORE_ASSERT(result, "Failed to get ExecPath");
  return result.value();
}

Expected<std::filesystem::path, errors::OSError> LinuxOSAPI::TryGetExecPath() const
{
  std::error_code ec;
  auto path = std::filesystem::canonical("/proc/self/exe", ec);

  if (ec) {
    return Unexpected(errors::OSError::ExecutablePathUnavailable);
  }

  return path;
}
} // namespace psych
