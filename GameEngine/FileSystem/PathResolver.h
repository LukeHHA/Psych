#pragma once

#include "FileSystem/EnginePath.h"
#include "FileSystem/FileSystem.h"

namespace ge::util
{
class PathResolver
{
public:
  PathResolver()                               = default;
  PathResolver(const PathResolver&)            = delete;
  PathResolver& operator=(const PathResolver&) = delete;
  PathResolver(PathResolver&&)                 = delete;
  PathResolver& operator=(PathResolver&&)      = delete;
  ~PathResolver()                              = default;

  void SetAssetRoot(FilePath path);
  void SetEngineRoot(FilePath path);
  void SetEditorRoot(FilePath path);
  void ClearAssetRoot();
  void ClearEngineRoot();
  void ClearEditorRoot();

  [[nodiscard]] Expected<FilePath, errors::FilesystemError> TryResolve(const EnginePath::Path& path) const;

private:
  FilePath m_AssetRoot_;
  FilePath m_EngineRoot_;
  FilePath m_EditorRoot_;
};
} // namespace ge::util
