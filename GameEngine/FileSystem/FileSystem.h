#pragma once

#include "Core/Core.h"
#include "Errors/Errors.h"
#include "OSFilesystemAPI.h"
#include "ge_expected"

#include <filesystem>

namespace ge::util
{
using FilePath                     = std::filesystem::path;
using DirPath                      = std::filesystem::path;

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

struct FileNode {
  FileNode() = default;
  FileNode(std::filesystem::path path, bool isDir) : isDir(isDir), path(std::move(path)), name(this->path.filename().string()) {}

  bool isDir = false;
  std::vector<Unique<FileNode>> children;
  std::filesystem::path path;
  std::string name;
};

class Filesystem
{
public:
  Filesystem()                             = delete;
  Filesystem(const Filesystem&)            = delete;
  Filesystem& operator=(const Filesystem&) = delete;
  Filesystem(Filesystem&&)                 = delete;
  Filesystem& operator=(Filesystem&&)      = delete;
  ~Filesystem()                            = delete;

  static void Init();
  static void Shutdown();
  static void DeleteFile(const std::filesystem::path& path);
  static Expected<void, errors::FilesystemError> TryDeleteFile(const std::filesystem::path& path);
  static bool FileExists(const std::filesystem::path& path);
  static bool DirExists(const std::filesystem::path& path);
  static bool CreateFile(const std::filesystem::path& path);
  static Expected<void, errors::FilesystemError> TryCreateFile(const std::filesystem::path& path);
  static Expected<void, errors::FilesystemError> TryCreateDirs(const std::filesystem::path& path);
  static std::string StreamFile(const std::string& path);
  static Expected<std::string, errors::FilesystemError> TryReadFile(const std::filesystem::path& path);
  static Unique<FileNode> CreateDirectoryTree(const DirPath& rootPath);
  static Expected<Unique<FileNode>, errors::FilesystemError> TryCreateDirectoryTree(const DirPath& rootPath);

  static bool IsInitialized();
  static Expected<DirPath, errors::FilesystemError> TryGetBaseConfigPath();
  static Expected<DirPath, errors::FilesystemError> TryGetBaseCachePath();
  static DirPath GetBaseConfigPath();
  static DirPath GetBaseCachePath();

private:
  inline static Shared<OSFilesystemAPI> s_OSFilesystemAPI_ = nullptr;
};
} // namespace ge::util
