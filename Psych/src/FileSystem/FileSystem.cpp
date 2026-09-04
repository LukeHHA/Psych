
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
#include <fstream>

namespace psych
{
void Filesystem::Init()
{
  s_CurrentPath_ = std::filesystem::current_path();

  if (s_OSFilesystemAPI_ == nullptr) {
#if defined(PSYCH_PLATFORM_MACOS)
    s_OSFilesystemAPI_ = CreateShared<MacOSFilesystemAPI>();
#elif defined(PSYCH_PLATFORM_LINUX)
    s_OSFilesystemAPI_ = CreateShared<LinuxFilesystemAPI>();
#else
    CORE_ASSERT(false, "Unknow Operating system. Unable to init filesystem")
#endif
  }

  s_PathResolver_ = CreateUnique<PathResolver>(s_CurrentPath_);
}

void Filesystem::Shutdown()
{
  s_PathResolver_.reset();
  s_OSFilesystemAPI_ = nullptr;
}

std::filesystem::path Filesystem::Current_Path() { return s_CurrentPath_; }

void Filesystem::DeleteFile(const std::filesystem::path& path) { CoreFilesystemAPI::DeleteFile(path); }

void Filesystem::DeleteFile(const EnginePath::Path& path)
{
  const auto result = TryDeleteFile(path);
  CORE_ASSERT(result, "Failed to delete engine path");
}

Expected<void, errors::FilesystemError> Filesystem::TryDeleteFile(const std::filesystem::path& path) { return CoreFilesystemAPI::TryDeleteFile(path); }

Expected<void, errors::FilesystemError> Filesystem::TryDeleteFile(const EnginePath::Path& path)
{
  const auto resolvedPath = TryResolve(path);
  if (!resolvedPath) {
    return Unexpected(resolvedPath.error());
  }

  return TryDeleteFile(resolvedPath.value());
}

bool Filesystem::FileExists(const std::filesystem::path& path) { return CoreFilesystemAPI::FileExists(path); }

bool Filesystem::FileExists(const EnginePath::Path& path)
{
  const auto resolvedPath = TryResolve(path);
  return resolvedPath && FileExists(resolvedPath.value());
}

bool Filesystem::DirExists(const std::filesystem::path& path) { return CoreFilesystemAPI::DirExists(path); }

bool Filesystem::DirExists(const EnginePath::Path& path)
{
  const auto resolvedPath = TryResolve(path);
  return resolvedPath && DirExists(resolvedPath.value());
}

bool Filesystem::CreateFile(const std::filesystem::path& path) { return CoreFilesystemAPI::CreateFile(path); }

bool Filesystem::CreateFile(const EnginePath::Path& path) { return static_cast<bool>(TryCreateFile(path)); }

Expected<void, errors::FilesystemError> Filesystem::TryCreateFile(const std::filesystem::path& path) { return CoreFilesystemAPI::TryCreateFile(path); }

Expected<void, errors::FilesystemError> Filesystem::TryCreateFile(const EnginePath::Path& path)
{
  const auto resolvedPath = TryResolve(path);
  if (!resolvedPath) {
    return Unexpected(resolvedPath.error());
  }

  return TryCreateFile(resolvedPath.value());
}

Expected<void, errors::FilesystemError> Filesystem::TryCreateDirs(const std::filesystem::path& path) { return CoreFilesystemAPI::TryCreateDirs(path); }

Expected<void, errors::FilesystemError> Filesystem::TryCreateDirs(const EnginePath::Path& path)
{
  const auto resolvedPath = TryResolve(path);
  if (!resolvedPath) {
    return Unexpected(resolvedPath.error());
  }

  return TryCreateDirs(resolvedPath.value());
}

std::string Filesystem::StreamFile(const std::filesystem::path& path)
{
  const auto result = TryReadFile(path);
  if (!result) {
    CORE_LOG_ERROR("Path: {}", path.string());
    CORE_ASSERT(false, "Failed to read file");
    return {};
  }

  return result.value();
}

std::string Filesystem::StreamFile(const EnginePath::Path& path)
{
  const auto result = TryReadFile(path);
  if (!result) {
    CORE_ASSERT(false, "Failed to read engine path");
    return {};
  }

  return result.value();
}

Expected<std::string, errors::FilesystemError> Filesystem::TryReadFile(const std::filesystem::path& path) { return CoreFilesystemAPI::TryReadFile(path); }

Expected<std::string, errors::FilesystemError> Filesystem::TryReadFile(const EnginePath::Path& path)
{
  const auto resolvedPath = TryResolve(path);
  if (!resolvedPath) {
    return Unexpected(resolvedPath.error());
  }

  return CoreFilesystemAPI::TryReadFile(resolvedPath.value());
}

Expected<std::filesystem::path, errors::FilesystemError> Filesystem::TryResolve(const EnginePath::Path& path)
{
  if (!s_PathResolver_) {
    return Unexpected(errors::FilesystemError::NotInitialized);
  }

  return s_PathResolver_->TryResolve(path);
}

bool Filesystem::IsInitialized() { return s_OSFilesystemAPI_ != nullptr && s_PathResolver_ != nullptr; }

Expected<std::filesystem::path, errors::FilesystemError> Filesystem::TryGetBaseConfigPath()
{
  if (!IsInitialized()) {
    return Unexpected(errors::FilesystemError::NotInitialized);
  }

  std::filesystem::path basePath = s_OSFilesystemAPI_->GetOSAppDataPath();
  if (basePath.empty()) {
    return Unexpected(errors::FilesystemError::OSPathFail);
  }

  std::filesystem::path configPath = basePath / PsychEngineName / "config";
  const auto result                = CoreFilesystemAPI::TryCreateDirs(configPath);
  if (!result) {
    return Unexpected(result.error());
  }

  return configPath;
}

std::filesystem::path Filesystem::GetBaseConfigPath()
{
  const auto result = TryGetBaseConfigPath();
  if (!result) {
    CORE_ASSERT(false, "Unable to get config directory")
    return {};
  }

  return result.value();
}

Expected<std::filesystem::path, errors::FilesystemError> Filesystem::TryGetBaseCachePath()
{
  if (!IsInitialized()) {
    return Unexpected(errors::FilesystemError::NotInitialized);
  }

  std::filesystem::path basePath = s_OSFilesystemAPI_->GetOSCacheDataPath();
  if (basePath.empty()) {
    return Unexpected(errors::FilesystemError::OSPathFail);
  }

  std::filesystem::path cachePath = basePath / PsychEngineName;

  const auto result               = CoreFilesystemAPI::TryCreateDirs(cachePath);
  if (!result) {
    return Unexpected(result.error());
  }

  return cachePath;
}

std::filesystem::path Filesystem::GetBaseCachePath()
{
  const auto result = TryGetBaseCachePath();
  if (!result) {
    CORE_ASSERT(false, "Unable to get cache directory")
    return {};
  }

  return result.value();
}

Expected<std::filesystem::path, errors::FilesystemError> Filesystem::TryGetBaseLogPath()
{
  if (!IsInitialized()) {
    return Unexpected(errors::FilesystemError::NotInitialized);
  }

  std::filesystem::path basePath = s_OSFilesystemAPI_->GetOSLogDataPath();
  if (basePath.empty()) {
    return Unexpected(errors::FilesystemError::OSPathFail);
  }

  std::filesystem::path logPath = basePath / PsychEngineName / "Logs";
  const auto result             = CoreFilesystemAPI::TryCreateDirs(logPath);
  if (!result) {
    return Unexpected(result.error());
  }

  return logPath;
}

std::filesystem::path Filesystem::GetBaseLogPath()
{
  const auto result = TryGetBaseLogPath();
  if (!result) {
    CORE_ASSERT(false, "Unable to get log directory")
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

Unique<FileNode> Filesystem::CreateDirectoryTree(const EnginePath::Path& path)
{
  auto result = TryCreateDirectoryTree(path);
  if (!result) {
    CORE_ASSERT(false, "Unable to create directory tree from engine path");
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

Expected<Unique<FileNode>, errors::FilesystemError> Filesystem::TryCreateDirectoryTree(const EnginePath::Path& path)
{
  const auto resolvedPath = TryResolve(path);
  if (!resolvedPath) {
    return Unexpected(resolvedPath.error());
  }

  return TryCreateDirectoryTree(resolvedPath.value());
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

std::vector<std::byte> Filesystem::ReadBinaryFile(const EnginePath::Path& path)
{
  const auto resolvedPath = TryResolve(path);
  if (!resolvedPath) {
    CORE_ASSERT(false, "Failed to resolve engine path for binary read");
    return {};
  }

  return ReadBinaryFile(resolvedPath.value());
}

std::filesystem::path Filesystem::GetFileExplorer() { return s_OSFilesystemAPI_->GetOSFileExplorer(); }
} // namespace psych
