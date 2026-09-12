#include "MacOSAPI.h"
#include "Debug/Assert.h"
#include "Errors/Errors.h"
#include <filesystem>
#include <mach-o/dyld.h>
#include <sys/param.h>
#include <system_error>
#include <vector>

namespace psych
{
std::filesystem::path MacOSAPI::GetExecPath() const
{
  auto result = TryGetExecPath();
  if (result) {
    return result.value();
  }
  CORE_ASSERT(false, "Failed to get ExecPath");
  return {};
}

Expected<std::filesystem::path, errors::OSError> MacOSAPI::TryGetExecPath() const
{
  std::vector<char> buffer(PATH_MAX);
  auto size = static_cast<uint32_t>(buffer.size());

  auto result = _NSGetExecutablePath(buffer.data(), &size);
  if (result != 0) {
    buffer.resize(size);
    result = _NSGetExecutablePath(buffer.data(), &size);
  }

  if (result != 0) {
    return Unexpected(errors::OSError::ExecutablePathUnavailable);
  }

  std::error_code error;
  auto path = std::filesystem::canonical(buffer.data(), error);
  if (error) {
    return Unexpected(errors::OSError::ExecutablePathResolutionFailed);
  }

  return path;
}

} // namespace psych
