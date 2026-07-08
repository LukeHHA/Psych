 
/**************************************************************************/
/*  FileSystem.cpp                                                        */                                                            
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

#include "FileSystem.h"
#include "Core/Core.h"
#include "Debug/Assert.h"
#include "FileSystem/CoreFilesystemAPI.h"
#include "Platform/LinuxFilesystemAPI.h"
#include "Platform/MacOSFilesystemAPI.h"
#include <filesystem>

namespace psych::util
{
void Filesystem::Init()
{
  s_CurrentPath_ = std::filesystem::current_path();

  if (s_OSFilesystemAPI_ == nullptr) {
#if defined(GE_PLATFORM_MACOS)
    s_OSFilesystemAPI_ = CreateShared<MacOSFilesystemAPI>();
#elif defined(GE_PLATFORM_LINUX)
    s_OSFilesystemAPI_ = CreateShared<LinuxFilesystemAPI>();
#else
    CORE_ASSERT(false, "Unknow Operating system. Unable to init filesystem")
#endif
  }
}

void Filesystem::Shutdown() { s_OSFilesystemAPI_ = nullptr; }

FilePath Filesystem::Current_Path() { return s_CurrentPath_; }

void Filesystem::DeleteFile(const std::filesystem::path& path) { CoreFilesystemAPI::DeleteFile(path); }

Expected<void, errors::FilesystemError> Filesystem::TryDeleteFile(const std::filesystem::path& path) { return CoreFilesystemAPI::TryDeleteFile(path); }

bool Filesystem::FileExists(const std::filesystem::path& path) { return CoreFilesystemAPI::FileExists(path); }

bool Filesystem::DirExists(const std::filesystem::path& path) { return CoreFilesystemAPI::DirExists(path); }

bool Filesystem::CreateFile(const std::filesystem::path& path) { return CoreFilesystemAPI::CreateFile(path); }

Expected<void, errors::FilesystemError> Filesystem::TryCreateFile(const std::filesystem::path& path) { return CoreFilesystemAPI::TryCreateFile(path); }

Expected<void, errors::FilesystemError> Filesystem::TryCreateDirs(const std::filesystem::path& path) { return CoreFilesystemAPI::TryCreateDirs(path); }

std::string Filesystem::StreamFile(const std::string& path) { return CoreFilesystemAPI::StreamFile(path); }

Expected<std::string, errors::FilesystemError> Filesystem::TryReadFile(const std::filesystem::path& path) { return CoreFilesystemAPI::TryReadFile(path); }

bool Filesystem::IsInitialized() { return s_OSFilesystemAPI_ != nullptr; }

Expected<DirPath, errors::FilesystemError> Filesystem::TryGetBaseConfigPath()
{
  if (!IsInitialized()) {
    return Unexpected(errors::FilesystemError::NotInitialized);
  }

  DirPath basePath = s_OSFilesystemAPI_->GetOSAppDataPath();
  if (basePath.empty()) {
    return Unexpected(errors::FilesystemError::OSPathFail);
  }

  DirPath configPath = basePath / PsychEngineName / "config";
  const auto result  = CoreFilesystemAPI::TryCreateDirs(configPath);
  if (!result) {
    return Unexpected(result.error());
  }

  return configPath;
}

DirPath Filesystem::GetBaseConfigPath()
{
  const auto result = TryGetBaseConfigPath();
  if (!result) {
    CORE_ASSERT(false, "Unable to get config directory")
    return {};
  }

  return result.value();
}

Expected<DirPath, errors::FilesystemError> Filesystem::TryGetBaseCachePath()
{
  if (!IsInitialized()) {
    return Unexpected(errors::FilesystemError::NotInitialized);
  }

  DirPath basePath = s_OSFilesystemAPI_->GetOSCacheDataPath();
  if (basePath.empty()) {
    return Unexpected(errors::FilesystemError::OSPathFail);
  }

  DirPath cachePath = basePath / PsychEngineName;

  const auto result = CoreFilesystemAPI::TryCreateDirs(cachePath);
  if (!result) {
    return Unexpected(result.error());
  }

  return cachePath;
}

DirPath Filesystem::GetBaseCachePath()
{
  const auto result = TryGetBaseCachePath();
  if (!result) {
    CORE_ASSERT(false, "Unable to get cache directory")
    return {};
  }

  return result.value();
}

Unique<FileNode> Filesystem::CreateDirectoryTree(const std::filesystem::path& path)
{
  auto result = TryCreateDirectoryTree(path);
  if (!result) {
    CORE_ASSERT(false, "Unable to create directory tree")
    return nullptr;
  }

  return std::move(result.value());
}

Expected<Unique<FileNode>, errors::FilesystemError> Filesystem::TryCreateDirectoryTree(const std::filesystem::path& path)
{
  if (path.empty()) {
    return Unexpected(errors::FilesystemError::InvalidPath);
  }

  std::error_code ec;
  const bool exists = std::filesystem::exists(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::DirectoryIterationFailed);
  }

  if (!exists) {
    return Unexpected(errors::FilesystemError::FileNotFound);
  }

  const bool isDir = std::filesystem::is_directory(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::DirectoryIterationFailed);
  }

  auto node = CreateUnique<FileNode>(path, isDir);

  if (!isDir) {
    return node;
  }

  std::filesystem::directory_iterator iterator(path, ec);
  if (ec) {
    return Unexpected(errors::FilesystemError::DirectoryIterationFailed);
  }

  for (const auto& entry : iterator) {
    auto child = TryCreateDirectoryTree(entry.path());
    if (!child) {
      return Unexpected(child.error());
    }

    node->children.emplace_back(std::move(child.value()));
  }

  return node;
}

std::vector<std::byte> Filesystem::ReadBinaryFile(const std::filesystem::path& path)
{
  std::ifstream file(path, std::ios::binary | std::ios::ate);

  if (!file) {
    CORE_ASSERT(false, "Failed to open file {}", path.string())
  }

  const auto size = file.tellg();
  if (size < 0) {
    CORE_ASSERT(false, "Failed to read file")
  }

  std::vector<std::byte> buffer(static_cast<std::size_t>(size));

  file.seekg(0, std::ios::beg);
  file.read(reinterpret_cast<char*>(buffer.data()), size);

  if (!file) {
    CORE_ASSERT(false, "Failed to read file")
  }

  return buffer;
}
} // namespace psych::util
