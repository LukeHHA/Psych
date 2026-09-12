#include "OS.h"
#include "expected.h"
#include <filesystem>

namespace psych
{
Expected<void, errors::OSError> OS::Init()
{
  s_OSAPI_ = OSAPI::Create();
  if (!s_OSAPI_) {
    return Unexpected(errors::OSError::FailedToInitialize);
  }
  return {};
}

Expected<void, errors::OSError> OS::Shutdown()
{
  s_OSAPI_.reset();
  return {};
}

std::filesystem::path OS::GetExecPath() { return s_OSAPI_->GetExecPath(); }
Expected<std::filesystem::path, errors::OSError> OS::TryGetExecPath() { return s_OSAPI_->TryGetExecPath(); }
} // namespace psych
