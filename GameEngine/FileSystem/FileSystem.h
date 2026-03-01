#pragma once

#include "Core/Base.h"
#include <filesystem>

namespace ge::util
{
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
  inline static std::filesystem::path s_CurrentWorkingDir_;
};
} // namespace ge::util
