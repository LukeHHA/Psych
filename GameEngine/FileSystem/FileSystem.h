#pragma once

#include "Core/Base.h"
#include "FileSystem/CoreFilesystemAPI.h"
#include "OSFilesystemAPI.h"
#include <filesystem>

namespace ge::util
{
using FilePath = std::filesystem::path;
using DirPath  = std::filesystem::path;

using recursive_directory_iterator =
    std::filesystem::recursive_directory_iterator;

struct FileNode {
  FileNode() = default;
  FileNode(std::filesystem::path path, bool isDir)
      : isDir(isDir), path(std::move(path)),
        name(this->path.filename().string())
  {
  }

  bool isDir = false;
  std::vector<Unique<FileNode>> children;
  std::filesystem::path path;
  std::string name;
};

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
  static std::string StreamFile(const std::string& path);
  static Unique<FileNode> CreateDirectoryTree(const DirPath& rootPath);

  static DirPath GetBaseConfigPath();
  static DirPath GetBaseCachePath();

private:
  inline static Shared<OSFilesystemAPI> s_OSFilesystemAPI_ = nullptr;
};
} // namespace ge::util
