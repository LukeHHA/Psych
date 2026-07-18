 
/**************************************************************************/
/*  CoreFilesystemAPI.cpp                                                 */                                                            
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

#include "CoreFilesystemAPI.h"
#include "Debug/Assert.h"
#include "FileSystem/OSFilesystemAPI.h"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace psych
{
bool CoreFilesystemAPI::FileExists(const std::filesystem::path& path)
{
  std::error_code ec;

  if (!std::filesystem::exists(path, ec)) {
    if (ec) {
      CORE_ASSERT(false, "exists() failed for '{}': {}", path.string(), ec.message());
    }
    return false;
  }

  if (!std::filesystem::is_regular_file(path, ec)) {
    if (ec) {
      CORE_ASSERT(false, "is_regular_file() failed for '{}': {}", path.string(), ec.message());
    }
    return false;
  }
  return true;
}

bool CoreFilesystemAPI::DirExists(const std::filesystem::path& path)
{
  std::error_code ec;
  if (!std::filesystem::exists(path, ec)) {
    if (ec) {
      CORE_ASSERT(false, "exists() failed for '{}': {}", path.string(), ec.message());
    }
    return false;
  }

  if (!std::filesystem::is_directory(path, ec)) {
    if (ec) {
      CORE_ASSERT(false, "is_directory() failed for '{}': {}", path.string(), ec.message());
    }
    return false;
  }
  return true;
}

void CoreFilesystemAPI::DeleteFile(const std::filesystem::path& path)
{
  const auto result = TryDeleteFile(path);
  CORE_ASSERT(result, "Failed to delete file '{}'", path.string());
}

Expected<void, errors::FilesystemError> CoreFilesystemAPI::TryDeleteFile(const std::filesystem::path& path)
{
  if (path.empty()) {
    return Unexpected(errors::FilesystemError::InvalidPath);
  }

  std::error_code ec;
  const bool exists = std::filesystem::exists(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::DeleteFailed);
  }

  if (!exists) {
    return {};
  }

  if (!std::filesystem::is_regular_file(path, ec)) {
    if (ec) {
      return Unexpected(errors::FilesystemError::DeleteFailed);
    }
    return Unexpected(errors::FilesystemError::PathExistsWithWrongType);
  }

  std::filesystem::remove(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::DeleteFailed);
  }

  return {};
}

bool CoreFilesystemAPI::CreateFile(const std::filesystem::path& path) { return static_cast<bool>(TryCreateFile(path)); }

Expected<void, errors::FilesystemError> CoreFilesystemAPI::TryCreateFile(const std::filesystem::path& path)
{
  if (path.empty()) {
    return Unexpected(errors::FilesystemError::InvalidPath);
  }

  std::error_code ec;
  const bool exists = std::filesystem::exists(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::FileCreationFailed);
  }

  if (exists) {
    if (std::filesystem::is_regular_file(path, ec)) {
      return {};
    }

    if (ec) {
      return Unexpected(errors::FilesystemError::FileCreationFailed);
    }
    return Unexpected(errors::FilesystemError::PathExistsWithWrongType);
  }

  const auto parent = path.parent_path();

  if (!parent.empty()) {
    const auto parentResult = TryCreateDirs(parent);
    if (!parentResult) {
      return Unexpected(parentResult.error());
    }
  }

  std::ofstream file(path);
  if (!file) {
    return Unexpected(errors::FilesystemError::FileCreationFailed);
  }

  return {};
}

std::string CoreFilesystemAPI::StreamFile(const std::string& path)
{
  const auto result = TryReadFile(path);
  if (!result) {
    CORE_LOG_ERROR("Path: {}", path);
    CORE_ASSERT(false, "Failed to read file");
    return {};
  }

  return result.value();
}

Expected<std::string, errors::FilesystemError> CoreFilesystemAPI::TryReadFile(const std::filesystem::path& path)
{
  if (path.empty()) {
    return Unexpected(errors::FilesystemError::InvalidPath);
  }

  std::error_code ec;
  const bool exists = std::filesystem::exists(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::ReadFailed);
  }

  if (!exists) {
    return Unexpected(errors::FilesystemError::FileNotFound);
  }

  if (!std::filesystem::is_regular_file(path, ec)) {
    if (ec) {
      return Unexpected(errors::FilesystemError::ReadFailed);
    }
    return Unexpected(errors::FilesystemError::PathExistsWithWrongType);
  }

  std::ifstream file(path);
  if (!file.is_open()) {
    return Unexpected(errors::FilesystemError::ReadFailed);
  }

  std::stringstream contents;
  contents << file.rdbuf();
  return contents.str();
}

bool CoreFilesystemAPI::CreateDir(const std::filesystem::path& path) { return std::filesystem::create_directory(path); }

bool CoreFilesystemAPI::CreateDirWithParentPerms(const std::filesystem::path& path, const std::filesystem::path& parent_path)
{
  return std::filesystem::create_directory(path, parent_path);
}

bool CoreFilesystemAPI::CreateDirs(const std::filesystem::path& path) { return static_cast<bool>(TryCreateDirs(path)); }

Expected<void, errors::FilesystemError> CoreFilesystemAPI::TryCreateDirs(const std::filesystem::path& path)
{
  if (path.empty()) {
    return Unexpected(errors::FilesystemError::InvalidPath);
  }

  std::error_code ec;
  const bool exists = std::filesystem::exists(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::DirectoryCreationFailed);
  }

  if (exists) {
    if (std::filesystem::is_directory(path, ec)) {
      return {};
    }

    if (ec) {
      return Unexpected(errors::FilesystemError::DirectoryCreationFailed);
    }
    return Unexpected(errors::FilesystemError::PathExistsWithWrongType);
  }

  std::filesystem::create_directories(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::DirectoryCreationFailed);
  }

  return {};
}

bool CoreFilesystemAPI::CreateDirsWithParentPerms(const std::filesystem::path& path) { return std::filesystem::create_directories(path); }

} // namespace psych
