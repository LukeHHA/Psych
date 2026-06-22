#include "FileSystem.h"
#include "Core/Core.h"
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

void Filesystem::DeleteFile(const std::filesystem::path& path) { CoreFilesystemAPI::DeleteFile(path); }

Expected<void, errors::FilesystemError> Filesystem::TryDeleteFile(const std::filesystem::path& path) { return CoreFilesystemAPI::TryDeleteFile(path); }

bool Filesystem::FileExists(const std::filesystem::path& path) { return CoreFilesystemAPI::FileExists(path); }

bool Filesystem::DirExists(const std::filesystem::path& path) { return CoreFilesystemAPI::DirExists(path); }

bool Filesystem::CreateFile(const std::filesystem::path& path) { return CoreFilesystemAPI::CreateFile(path); }

Expected<void, errors::FilesystemError> Filesystem::TryCreateFile(const std::filesystem::path& path) { return CoreFilesystemAPI::TryCreateFile(path); }

Expected<void, errors::FilesystemError> Filesystem::TryCreateDirs(const std::filesystem::path& path) { return CoreFilesystemAPI::TryCreateDirs(path); }

std::string Filesystem::StreamFile(const std::string& path) { return CoreFilesystemAPI::StreamFile(path); }

Expected<std::string, errors::FilesystemError> Filesystem::TryReadFile(const std::filesystem::path& path) { return CoreFilesystemAPI::TryReadFile(path); }

bool Filesystem::IsInitialized() { return s_OSFilesystemAPI_ != nullptr; }

Expected<DirPath, errors::FilesystemError> Filesystem::TryGetBaseConfigPath()
{
  if (!IsInitialized()) {
    return Unexpected(errors::FilesystemError::NotInitialized);
  }

  DirPath basePath   = s_OSFilesystemAPI_->GetOSAppDataPath();
  DirPath configPath = basePath / GameEngineName / "config";
  const auto result  = CoreFilesystemAPI::TryCreateDirs(configPath);
  if (!result) {
    return Unexpected(result.error());
  }

  return configPath;
}

DirPath Filesystem::GetBaseConfigPath()
{
  const auto result = TryGetBaseConfigPath();
  if (!result) {
    CORE_ASSERT(false, "Unable to get config directory")
    return {};
  }

  return result.value();
}

Expected<DirPath, errors::FilesystemError> Filesystem::TryGetBaseCachePath()
{
  if (!IsInitialized()) {
    return Unexpected(errors::FilesystemError::NotInitialized);
  }

  DirPath basePath  = s_OSFilesystemAPI_->GetOSCacheDataPath();
  DirPath cachePath = basePath / GameEngineName;

  const auto result = CoreFilesystemAPI::TryCreateDirs(cachePath);
  if (!result) {
    return Unexpected(result.error());
  }

  return cachePath;
}

DirPath Filesystem::GetBaseCachePath()
{
  const auto result = TryGetBaseCachePath();
  if (!result) {
    CORE_ASSERT(false, "Unable to get cache directory")
    return {};
  }

  return result.value();
}

Unique<FileNode> Filesystem::CreateDirectoryTree(const std::filesystem::path& path)
{
  auto result = TryCreateDirectoryTree(path);
  if (!result) {
    CORE_ASSERT(false, "Unable to create directory tree")
    return nullptr;
  }

  return std::move(result.value());
}

Expected<Unique<FileNode>, errors::FilesystemError> Filesystem::TryCreateDirectoryTree(const std::filesystem::path& path)
{
  if (path.empty()) {
    return Unexpected(errors::FilesystemError::InvalidPath);
  }

  std::error_code ec;
  const bool exists = std::filesystem::exists(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::DirectoryIterationFailed);
  }

  if (!exists) {
    return Unexpected(errors::FilesystemError::FileNotFound);
  }

  const bool isDir = std::filesystem::is_directory(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::DirectoryIterationFailed);
  }

  auto node = CreateUnique<FileNode>(path, isDir);

  if (!isDir) {
    return node;
  }

  std::filesystem::directory_iterator iterator(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::DirectoryIterationFailed);
  }

  for (const auto& entry : iterator) {
    auto child = TryCreateDirectoryTree(entry.path());
    if (!child) {
      return Unexpected(child.error());
    }

    node->children.emplace_back(std::move(child.value()));
  }

  return node;
}

} // namespace ge::util
