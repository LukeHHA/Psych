 
/**************************************************************************/
/*  CoreFilesystemAPI.h                                                   */                                                            
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

#include "Errors/Errors.h"
#include "FileSystem/OSFilesystemAPI.h"
#include "expected.h"

#include <filesystem>

namespace psych
{
class CoreFilesystemAPI
{
public:
  static bool FileExists(const std::filesystem::path& path);

  static bool DirExists(const std::filesystem::path& path);

  static void DeleteFile(const std::filesystem::path& path);

  static Expected<void, errors::FilesystemError> TryDeleteFile(const std::filesystem::path& path);

  static std::string StreamFile(const std::string& path);

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
} // namespace psych
