#include "FileSystem.h"
#include "Logging/Logging.h"
#include <filesystem>
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
  if (Exists(path)) {
    std::filesystem::remove(path);
  }
}

bool Filesystem::Exists(const std::filesystem::path& path)
{
  return std::filesystem::exists(path);
}
} // namespace ge::util
