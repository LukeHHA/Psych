#include "PathResolver.h"

#include <utility>

namespace ge::util
{
void PathResolver::SetAssetRoot(FilePath path) { m_AssetRoot_ = std::move(path); }

void PathResolver::SetEngineRoot(FilePath path) { m_EngineRoot_ = std::move(path); }

void PathResolver::SetEditorRoot(FilePath path) { m_EditorRoot_ = std::move(path); }

void PathResolver::ClearAssetRoot() { m_AssetRoot_.clear(); }

void PathResolver::ClearEngineRoot() { m_EngineRoot_.clear(); }

void PathResolver::ClearEditorRoot() { m_EditorRoot_.clear(); }

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

  case EnginePath::Schema::Assets:
    if (m_AssetRoot_.empty()) {
      return Unexpected(errors::FilesystemError::InvalidPath);
    }
    return m_AssetRoot_ / relativePath;

  case EnginePath::Schema::Engine:
    if (m_EngineRoot_.empty()) {
      return Unexpected(errors::FilesystemError::InvalidPath);
    }
    return m_EngineRoot_ / relativePath;

  case EnginePath::Schema::Editor:
    if (m_EditorRoot_.empty()) {
      return Unexpected(errors::FilesystemError::InvalidPath);
    }
    return m_EditorRoot_ / relativePath;

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
} // namespace ge::util
