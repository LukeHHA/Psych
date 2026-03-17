#pragma once

#include "Core/Base.h"
#include <filesystem>

namespace ge::util
{
using FilePath = std::filesystem::path;
using recursive_directory_iterator =
    std::filesystem::recursive_directory_iterator;
class Filesystem
{
public:
  Filesystem()  = delete;
  ~Filesystem() = delete;
  CORE_DELETE_COPY_OPS(Filesystem);

  static void Init();
  static void Shutdown();
  static bool FileExists(const std::filesystem::path& path);
  static bool DirExists(const std::filesystem::path& path);
  static void DeleteFile(const std::filesystem::path& path);
  static const std::string StreamFile(const std::string& path);

private:
  static std::filesystem::path ResolvePath(const std::filesystem::path& path);

private:
  inline static std::filesystem::path s_CurrentWorkingDir_;
  inline static std::filesystem::path s_DataDirectory_;
};
} // namespace ge::util
