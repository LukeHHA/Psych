#include "FileSystem.h"
#include "Debug/Assert.h"
#include "Logging/Logging.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace ge::util
{
void Filesystem::Init()
{
  s_CurrentWorkingDir_ = std::filesystem::current_path();
}

void Filesystem::Shutdown() {}

void Filesystem::DeleteFile(const std::filesystem::path& path)
{
  if (FileExists(path)) {
    std::filesystem::remove(path);
  }
}

bool Filesystem::FileExists(const std::filesystem::path& path)
{
  std::error_code ec;
  const std::filesystem::path resolvedPath = ResolvePath(path);
  auto res = std::filesystem::is_regular_file(resolvedPath, ec);
  CORE_ASSERT(ec, "{}: FileExists Failed", ec.message());
  return res;
}

bool Filesystem::DirExists(const std::filesystem::path& path)
{
  std::error_code ec;
  const std::filesystem::path resolvedPath = ResolvePath(path);
  auto res = std::filesystem::is_directory(resolvedPath, ec);
  CORE_ASSERT(ec, "{}: DirectoryExists Failed", ec.message());
  return res;
}

const std::string Filesystem::StreamFile(const std::string& path)
{
  const std::filesystem::path resolvedPath = ResolvePath(path);
  std::ifstream file(resolvedPath);
  if (!file.is_open()) {
    CORE_LOG_ERROR("Path: {}", resolvedPath.string());
    CORE_ASSERT(false, "Failed to open file");
  }

  std::stringstream contents;
  contents << file.rdbuf();
  return contents.str();
}

std::filesystem::path Filesystem::ResolvePath(const std::filesystem::path& path)
{
  if (path.is_absolute()) {
    return path;
  }

  if (!s_DataDirectory_.empty()) {
    return s_DataDirectory_ / path;
  }

  return s_CurrentWorkingDir_ / path;
}
} // namespace ge::util
