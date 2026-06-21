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
      CORE_ASSERT(false, "exists() failed for '{}': {}", path.string(),
                  ec.message());
    }
    return false;
  }

  if (!std::filesystem::is_regular_file(path, ec)) {
    if (ec) {
      CORE_ASSERT(false, "is_regular_file() failed for '{}': {}", path.string(),
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
      CORE_ASSERT(false, "exists() failed for '{}': {}", path.string(),
                  ec.message());
    }
    return false;
  }

  if (!std::filesystem::is_directory(path, ec)) {
    if (ec) {
      CORE_ASSERT(false, "is_directory() failed for '{}': {}", path.string(),
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
  return std::filesystem::create_directories(path);
}

bool CoreFilesystemAPI::CreateDirsWithParentPerms(const DirPath& path)
{
  return std::filesystem::create_directories(path);
}

} // namespace ge
