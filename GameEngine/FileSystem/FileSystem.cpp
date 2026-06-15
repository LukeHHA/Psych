#include "FileSystem.h"
#include "Core/Base.h"
#include "Debug/Assert.h"
#include "FileSystem/CoreFilesystemAPI.h"
#include "Platform/MacOSFilesystemAPI.h"
#include <filesystem>

namespace ge::util
{
void Filesystem::Init()
{
  if (s_OSFilesystemAPI_ == nullptr) {
#if defined(GE_PLATFORM_MACOS)
    s_OSFilesystemAPI_ = CreateShared<MacOSFilesystemAPI>();
#else
    CORE_ASSERT(false, "Unknow Operating system. Unable to init filesystem")
#endif
  }
}

void Filesystem::Shutdown() { s_OSFilesystemAPI_ = nullptr; }

void Filesystem::DeleteFile(const std::filesystem::path& path)
{
  CoreFilesystemAPI::DeleteFile(path);
}

bool Filesystem::FileExists(const std::filesystem::path& path)
{
  return CoreFilesystemAPI::FileExists(path);
}

bool Filesystem::DirExists(const std::filesystem::path& path)
{
  return CoreFilesystemAPI::DirExists(path);
}

std::string Filesystem::StreamFile(const std::string& path)
{
  return CoreFilesystemAPI::StreamFile(path);
}

DirPath Filesystem::GetBaseConfigPath()
{
  DirPath basePath   = s_OSFilesystemAPI_->GetOSAppDataPath();
  DirPath configPath = basePath / GameEngineName / "config";
  if (CoreFilesystemAPI::DirExists(configPath)) {
    return configPath;
  }

  if (!CoreFilesystemAPI::CreateDirs(configPath)) {
    CORE_ASSERT(false, "Unable to create config directory")
    return {};
  }

  return {};
}

DirPath Filesystem::GetBaseCachePath()
{
  DirPath basePath  = s_OSFilesystemAPI_->GetOSCacheDataPath();
  DirPath cachePath = basePath / GameEngineName;

  if (CoreFilesystemAPI::DirExists(cachePath)) {
    return cachePath;
  }

  if (!CoreFilesystemAPI::CreateDirs(cachePath)) {
    CORE_ASSERT(false, "Unable to create a cache directory")
    return {};
  }

  return cachePath;
}

Unique<FileNode>
Filesystem::CreateDirectoryTree(const std::filesystem::path& path)
{
  const bool isDir = std::filesystem::is_directory(path);

  auto node        = CreateUnique<FileNode>(path, isDir);

  if (!isDir) {
    return node;
  }

  for (const auto& entry : std::filesystem::directory_iterator(path)) {
    node->children.emplace_back(CreateDirectoryTree(entry.path()));
  }

  return node;
}

} // namespace ge::util
