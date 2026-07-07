#include "PathResolver.h"
#include "FileSystem/FileSystem.h"
#include <filesystem>

namespace psych::util
{
PathResolver::PathResolver() { m_EngineRoot_ = util::Filesystem::Current_Path(); }

Expected<FilePath, errors::FilesystemError> PathResolver::TryResolve(const EnginePath::Path& path) const
{
  if (!path.IsValid()) {
    return Unexpected(errors::FilesystemError::InvalidPath);
  }

  const auto& relativePath = path.GetRelativePath();

  switch (path.GetSchema()) {
  case EnginePath::Schema::Config: {
    auto baseConfigPath = Filesystem::TryGetBaseConfigPath();
    if (!baseConfigPath) {
      return Unexpected(errors::FilesystemError::OSPathFail);
    }
    return baseConfigPath.value() / relativePath;
  }

  case EnginePath::Schema::Engine:
    if (m_EngineRoot_.empty()) {
      return Unexpected(errors::FilesystemError::InvalidPath);
    }
    return m_EngineRoot_ / relativePath;

  case EnginePath::Schema::Cache: {
    auto baseCachePath = Filesystem::TryGetBaseCachePath();
    if (!baseCachePath) {
      return Unexpected(errors::FilesystemError::OSPathFail);
    }
    return baseCachePath.value() / relativePath;
  }

  case EnginePath::Schema::None:
    break;
  }

  return Unexpected(errors::FilesystemError::InvalidPath);
}
} // namespace psych::util
