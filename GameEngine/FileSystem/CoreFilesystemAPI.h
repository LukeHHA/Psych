#pragma once

#include "FileSystem/OSFilesystemAPI.h"
#include <filesystem>
namespace ge
{
class CoreFilesystemAPI
{
public:
  static bool FileExists(const std::filesystem::path& path);

  static bool DirExists(const std::filesystem::path& path);

  static void DeleteFile(const std::filesystem::path& path);

  static const std::string StreamFile(const std::string& path);

  static bool CreateDir(const DirPath& path);

  static bool CreateDirWithParentPerms(const DirPath& path,
                                       const DirPath& parent_path);

  static bool CreateDirs(const DirPath& path);

  /// NOTE:Function needs to be updated to handle perms internall for
  /// create_directories
  [[deprecated]]
  static bool CreateDirsWithParentPerms(const DirPath& path);
};
} // namespace ge
