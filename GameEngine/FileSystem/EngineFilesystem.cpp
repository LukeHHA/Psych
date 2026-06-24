#include "EngineFilesystem.h"

#include "Debug/Assert.h"

namespace ge::util
{
void EngineFilesystem::Init(PathResolver& resolver) { s_PathResolver_ = &resolver; }

void EngineFilesystem::Shutdown() { s_PathResolver_ = nullptr; }

bool EngineFilesystem::IsInitialized() { return s_PathResolver_ != nullptr; }

PathResolver* EngineFilesystem::Resolver()
{
  CORE_ASSERT(s_PathResolver_, "EngineFilesystem used before initialization")
  return s_PathResolver_;
}

Expected<FilePath, errors::FilesystemError> EngineFilesystem::TryResolve(const EnginePath::Path& path)
{
  const auto* resolver = Resolver();
  if (!resolver) {
    return Unexpected(errors::FilesystemError::NotInitialized);
  }

  return resolver->TryResolve(path);
}

FilePath EngineFilesystem::Resolve(const EnginePath::Path& path)
{
  const auto result = TryResolve(path);
  if (!result) {
    CORE_ASSERT(false, "Failed to resolve engine path")
    return {};
  }

  return result.value();
}

FilePath EngineFilesystem::Resolve(const std::string& path) { return Resolve(EnginePath::Path{path}); }

FilePath EngineFilesystem::Resolve(const char* path) { return Resolve(EnginePath::Path{path}); }

Expected<FilePath, errors::FilesystemError> EngineFilesystem::TryResolve(const std::string& path) { return TryResolve(EnginePath::Path{path}); }

Expected<FilePath, errors::FilesystemError> EngineFilesystem::TryResolve(const char* path) { return TryResolve(EnginePath::Path{path}); }

bool EngineFilesystem::FileExists(const EnginePath::Path& path)
{
  const auto result = TryFileExists(path);
  if (!result) {
    CORE_ASSERT(false, "Failed to resolve engine path")
    return false;
  }

  return result.value();
}

bool EngineFilesystem::FileExists(const std::string& path) { return FileExists(EnginePath::Path{path}); }

bool EngineFilesystem::FileExists(const char* path) { return FileExists(EnginePath::Path{path}); }

Expected<bool, errors::FilesystemError> EngineFilesystem::TryFileExists(const EnginePath::Path& path)
{
  const auto resolvedPath = TryResolve(path);
  if (!resolvedPath) {
    return Unexpected(resolvedPath.error());
  }

  return Filesystem::FileExists(resolvedPath.value());
}

Expected<bool, errors::FilesystemError> EngineFilesystem::TryFileExists(const std::string& path) { return TryFileExists(EnginePath::Path{path}); }

Expected<bool, errors::FilesystemError> EngineFilesystem::TryFileExists(const char* path) { return TryFileExists(EnginePath::Path{path}); }

bool EngineFilesystem::DirExists(const EnginePath::Path& path)
{
  const auto result = TryDirExists(path);
  if (!result) {
    CORE_ASSERT(false, "Failed to resolve engine path")
    return false;
  }

  return result.value();
}

bool EngineFilesystem::DirExists(const std::string& path) { return DirExists(EnginePath::Path{path}); }

bool EngineFilesystem::DirExists(const char* path) { return DirExists(EnginePath::Path{path}); }

Expected<bool, errors::FilesystemError> EngineFilesystem::TryDirExists(const EnginePath::Path& path)
{
  const auto resolvedPath = TryResolve(path);
  if (!resolvedPath) {
    return Unexpected(resolvedPath.error());
  }

  return Filesystem::DirExists(resolvedPath.value());
}

Expected<bool, errors::FilesystemError> EngineFilesystem::TryDirExists(const std::string& path) { return TryDirExists(EnginePath::Path{path}); }

Expected<bool, errors::FilesystemError> EngineFilesystem::TryDirExists(const char* path) { return TryDirExists(EnginePath::Path{path}); }

std::string EngineFilesystem::ReadFile(const EnginePath::Path& path)
{
  const auto result = TryReadFile(path);
  if (!result) {
    CORE_ASSERT(false, "Failed to read engine path file")
    return {};
  }

  return result.value();
}

std::string EngineFilesystem::ReadFile(const std::string& path) { return ReadFile(EnginePath::Path{path}); }

std::string EngineFilesystem::ReadFile(const char* path) { return ReadFile(EnginePath::Path{path}); }

Expected<std::string, errors::FilesystemError> EngineFilesystem::TryReadFile(const EnginePath::Path& path)
{
  const auto resolvedPath = TryResolve(path);
  if (!resolvedPath) {
    return Unexpected(resolvedPath.error());
  }

  return Filesystem::TryReadFile(resolvedPath.value());
}

Expected<std::string, errors::FilesystemError> EngineFilesystem::TryReadFile(const std::string& path) { return TryReadFile(EnginePath::Path{path}); }

Expected<std::string, errors::FilesystemError> EngineFilesystem::TryReadFile(const char* path) { return TryReadFile(EnginePath::Path{path}); }

Unique<FileNode> EngineFilesystem::CreateDirectoryTree(const EnginePath::Path& rootPath)
{
  auto result = TryCreateDirectoryTree(rootPath);
  if (!result) {
    CORE_ASSERT(false, "Failed to create engine path directory tree")
    return nullptr;
  }

  return std::move(result.value());
}

Unique<FileNode> EngineFilesystem::CreateDirectoryTree(const std::string& rootPath) { return CreateDirectoryTree(EnginePath::Path{rootPath}); }

Unique<FileNode> EngineFilesystem::CreateDirectoryTree(const char* rootPath) { return CreateDirectoryTree(EnginePath::Path{rootPath}); }

Expected<Unique<FileNode>, errors::FilesystemError> EngineFilesystem::TryCreateDirectoryTree(const EnginePath::Path& rootPath)
{
  const auto resolvedPath = TryResolve(rootPath);
  if (!resolvedPath) {
    return Unexpected(resolvedPath.error());
  }

  return Filesystem::TryCreateDirectoryTree(resolvedPath.value());
}

Expected<Unique<FileNode>, errors::FilesystemError> EngineFilesystem::TryCreateDirectoryTree(const std::string& rootPath)
{
  return TryCreateDirectoryTree(EnginePath::Path{rootPath});
}

Expected<Unique<FileNode>, errors::FilesystemError> EngineFilesystem::TryCreateDirectoryTree(const char* rootPath)
{
  return TryCreateDirectoryTree(EnginePath::Path{rootPath});
}
} // namespace ge::util
