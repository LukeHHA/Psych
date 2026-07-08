 
/**************************************************************************/
/*  FileSystem.h                                                          */                                                            
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PSYCH ENGINE                               */
/**************************************************************************/
/* Copyright (c)  Luke Howe                                               */                                                  
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "Core/Core.h"
#include "Errors/Errors.h"
#include "OSFilesystemAPI.h"
#include "expected.h"

#include <filesystem>
#include <fstream>

namespace psych::util
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

  static std::vector<std::byte> ReadBinaryFile(const std::filesystem::path& path);
  static void Init();
  static void Shutdown();
  static FilePath Current_Path();
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
  inline static std::filesystem::path s_CurrentPath_;
};
} // namespace psych::util
