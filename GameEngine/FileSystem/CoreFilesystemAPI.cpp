#include "CoreFilesystemAPI.h"
#include "Debug/Assert.h"
#include "FileSystem/OSFilesystemAPI.h"
#include <filesystem>
#include <fstream>

namespace ge
{
bool CoreFilesystemAPI::FileExists(const std::filesystem::path& path)
{
  std::error_code ec;

  if (!std::filesystem::exists(path, ec)) {
    if (ec) {
      CORE_ASSERT(false,
                  "exists() failed for '{}': {}",
                  path.string(),
                  ec.message());
    }
    return false;
  }

  if (!std::filesystem::is_regular_file(path, ec)) {
    if (ec) {
      CORE_ASSERT(false,
                  "is_regular_file() failed for '{}': {}",
                  path.string(),
                  ec.message());
    }
    return false;
  }
  return true;
}

bool CoreFilesystemAPI::DirExists(const std::filesystem::path& path)
{
  std::error_code ec;
  if (!std::filesystem::exists(path, ec)) {
    if (ec) {
      CORE_ASSERT(false,
                  "exists() failed for '{}': {}",
                  path.string(),
                  ec.message());
    }
    return false;
  }

  if (!std::filesystem::is_directory(path, ec)) {
    if (ec) {
      CORE_ASSERT(false,
                  "is_directory() failed for '{}': {}",
                  path.string(),
                  ec.message());
    }
    return false;
  }
  return true;
}

void CoreFilesystemAPI::DeleteFile(const std::filesystem::path& path)
{
  if (FileExists(path)) {
    std::filesystem::remove(path);
  }
}

bool CoreFilesystemAPI::CreateFile(const std::filesystem::path& path)
{
  return static_cast<bool>(TryCreateFile(path));
}

Expected<void, errors::FilesystemError>
CoreFilesystemAPI::TryCreateFile(const FilePath& path)
{
  if (path.empty()) {
    return Unexpected(errors::FilesystemError::InvalidPath);
  }

  std::error_code ec;
  const bool exists = std::filesystem::exists(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::FileCreationFailed);
  }

  if (exists) {
    if (std::filesystem::is_regular_file(path, ec)) {
      return {};
    }

    if (ec) {
      return Unexpected(errors::FilesystemError::FileCreationFailed);
    }
    return Unexpected(errors::FilesystemError::PathExistsWithWrongType);
  }

  const auto parent = path.parent_path();

  if (!parent.empty()) {
    const auto parentResult = TryCreateDirs(parent);
    if (!parentResult) {
      return Unexpected(parentResult.error());
    }
  }

  std::ofstream file(path);
  if (!file) {
    return Unexpected(errors::FilesystemError::FileCreationFailed);
  }

  return {};
}

const std::string CoreFilesystemAPI::StreamFile(const std::string& path)
{
  std::ifstream file(path);
  if (!file.is_open()) {
    CORE_LOG_ERROR("Path: {}", path);
    CORE_ASSERT(false, "Failed to open file");
  }

  std::stringstream contents;
  contents << file.rdbuf();
  return contents.str();
}

bool CoreFilesystemAPI::CreateDir(const DirPath& path)
{
  return std::filesystem::create_directory(path);
}

bool CoreFilesystemAPI::CreateDirWithParentPerms(const DirPath& path,
                                                 const DirPath& parent_path)
{
  return std::filesystem::create_directory(path, parent_path);
}

bool CoreFilesystemAPI::CreateDirs(const DirPath& path)
{
  return static_cast<bool>(TryCreateDirs(path));
}

Expected<void, errors::FilesystemError>
CoreFilesystemAPI::TryCreateDirs(const DirPath& path)
{
  if (path.empty()) {
    return Unexpected(errors::FilesystemError::InvalidPath);
  }

  std::error_code ec;
  const bool exists = std::filesystem::exists(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::DirectoryCreationFailed);
  }

  if (exists) {
    if (std::filesystem::is_directory(path, ec)) {
      return {};
    }

    if (ec) {
      return Unexpected(errors::FilesystemError::DirectoryCreationFailed);
    }
    return Unexpected(errors::FilesystemError::PathExistsWithWrongType);
  }

  std::filesystem::create_directories(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::DirectoryCreationFailed);
  }

  return {};
}

bool CoreFilesystemAPI::CreateDirsWithParentPerms(const DirPath& path)
{
  return std::filesystem::create_directories(path);
}

} // namespace ge
