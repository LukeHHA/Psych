#pragma once

#include "Errors/Errors.h"
#include "FileSystem/OSFilesystemAPI.h"
#include "ge_expected"

#include <filesystem>

namespace ge
{
class CoreFilesystemAPI
{
public:
  static bool FileExists(const std::filesystem::path& path);

  static bool DirExists(const std::filesystem::path& path);

  static void DeleteFile(const std::filesystem::path& path);

  static Expected<void, errors::FilesystemError> TryDeleteFile(const std::filesystem::path& path);

  static const std::string StreamFile(const std::string& path);

  static Expected<std::string, errors::FilesystemError> TryReadFile(const std::filesystem::path& path);

  static bool CreateDir(const DirPath& path);

  static bool CreateDirWithParentPerms(const DirPath& path, const DirPath& parent_path);

  static bool CreateDirs(const DirPath& path);

  static bool CreateFile(const FilePath& path);

  static Expected<void, errors::FilesystemError> TryCreateDirs(const DirPath& path);

  static Expected<void, errors::FilesystemError> TryCreateFile(const FilePath& path);

  /// NOTE:Function needs to be updated to handle perms internall for
  /// create_directories
  [[deprecated]]
  static bool CreateDirsWithParentPerms(const DirPath& path);
};
} // namespace ge
