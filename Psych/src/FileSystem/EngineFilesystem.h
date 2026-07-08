 
/**************************************************************************/
/*  EngineFilesystem.h                                                    */                                                            
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

#include "FileSystem/EnginePath.h"
#include "FileSystem/FileSystem.h"
#include "FileSystem/PathResolver.h"

#include <string>

namespace psych::util
{
class EngineFilesystem
{
public:
  EngineFilesystem()                                   = delete;
  EngineFilesystem(const EngineFilesystem&)            = delete;
  EngineFilesystem& operator=(const EngineFilesystem&) = delete;
  EngineFilesystem(EngineFilesystem&&)                 = delete;
  EngineFilesystem& operator=(EngineFilesystem&&)      = delete;
  ~EngineFilesystem()                                  = delete;

  static void Init(PathResolver& resolver);
  static void Shutdown();
  [[nodiscard]] static bool IsInitialized();

  static FilePath Resolve(const EnginePath::Path& path);
  static FilePath Resolve(const std::string& path);
  static FilePath Resolve(const char* path);
  static Expected<FilePath, errors::FilesystemError> TryResolve(const EnginePath::Path& path);
  static Expected<FilePath, errors::FilesystemError> TryResolve(const std::string& path);
  static Expected<FilePath, errors::FilesystemError> TryResolve(const char* path);

  static bool FileExists(const EnginePath::Path& path);
  static bool FileExists(const std::string& path);
  static bool FileExists(const char* path);
  static Expected<bool, errors::FilesystemError> TryFileExists(const EnginePath::Path& path);
  static Expected<bool, errors::FilesystemError> TryFileExists(const std::string& path);
  static Expected<bool, errors::FilesystemError> TryFileExists(const char* path);

  static bool DirExists(const EnginePath::Path& path);
  static bool DirExists(const std::string& path);
  static bool DirExists(const char* path);
  static Expected<bool, errors::FilesystemError> TryDirExists(const EnginePath::Path& path);
  static Expected<bool, errors::FilesystemError> TryDirExists(const std::string& path);
  static Expected<bool, errors::FilesystemError> TryDirExists(const char* path);

  static std::string ReadFile(const EnginePath::Path& path);
  static std::string ReadFile(const std::string& path);
  static std::string ReadFile(const char* path);
  static Expected<std::string, errors::FilesystemError> TryReadFile(const EnginePath::Path& path);
  static Expected<std::string, errors::FilesystemError> TryReadFile(const std::string& path);
  static Expected<std::string, errors::FilesystemError> TryReadFile(const char* path);

  static Unique<FileNode> CreateDirectoryTree(const EnginePath::Path& rootPath);
  static Unique<FileNode> CreateDirectoryTree(const std::string& rootPath);
  static Unique<FileNode> CreateDirectoryTree(const char* rootPath);
  static Expected<Unique<FileNode>, errors::FilesystemError> TryCreateDirectoryTree(const EnginePath::Path& rootPath);
  static Expected<Unique<FileNode>, errors::FilesystemError> TryCreateDirectoryTree(const std::string& rootPath);
  static Expected<Unique<FileNode>, errors::FilesystemError> TryCreateDirectoryTree(const char* rootPath);

private:
  [[nodiscard]] static PathResolver* Resolver();

private:
  inline static PathResolver* s_PathResolver_ = nullptr;
};
} // namespace psych::util
