#pragma once

#include "FileSystem/EnginePath.h"
#include "FileSystem/FileSystem.h"

namespace psych::util
{
class PathResolver
{
public:
  PathResolver();
  PathResolver(const PathResolver&)            = delete;
  PathResolver& operator=(const PathResolver&) = delete;
  PathResolver(PathResolver&&)                 = delete;
  PathResolver& operator=(PathResolver&&)      = delete;
  ~PathResolver()                              = default;

  [[nodiscard]] Expected<FilePath, errors::FilesystemError> TryResolve(const EnginePath::Path& path) const;

private:
  FilePath m_EngineRoot_;
};
} // namespace psych::util
