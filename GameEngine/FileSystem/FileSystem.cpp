#include "FileSystem.h"
#include "Debug/Assert.h"
#include "Logging/Logging.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace ge::util
{
void Filesystem::Init()
{
  s_CurrentWorkingDir_ = std::filesystem::current_path();
  std::cout << "CWD: " + s_CurrentWorkingDir_.string() << "\n";
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
  std::filesystem::path abs_path;

  if (path.is_relative()) {
    abs_path = s_CurrentWorkingDir_ / path;
  }
  abs_path = path;

  auto res = std::filesystem::is_regular_file(abs_path, ec);
  CORE_ASSERT(ec, "{}: FileExists Failed", ec.message());
  return res;
}

bool Filesystem::DirExists(const std::filesystem::path& path)
{
  std::error_code ec;
  std::filesystem::path abs_path;

  if (path.is_relative()) {
    abs_path = s_CurrentWorkingDir_ / path;
  }
  abs_path = path;

  auto res = std::filesystem::is_directory(abs_path, ec);
  CORE_ASSERT(ec, "{}: DirectoryExists Failed", ec.message());
  return res;
}

const std::string StreamFile(const std::string& path)
{
  std::ifstream file(path);
  if (!file.is_open()) {
    CORE_ASSERT(false, "Failed to open file {}", path);
  }

  std::stringstream contents;
  contents << file.rdbuf();
  return contents.str();
}
} // namespace ge::util
