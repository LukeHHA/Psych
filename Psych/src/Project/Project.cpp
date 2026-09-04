
/**************************************************************************/
/*  Project.cpp                                                           */
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

#include "Project.h"

#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "FileSystem/FileSystem.h"
#include "cereal/archives/xml.hpp"

#include <sstream>
#include <utility>

namespace psych
{
Project::Project(const EnginePath::Path& path) : m_ProjectPath_(path) {}

Project::Project(const EnginePath::Path& path, ProjectConfig config) : m_ProjectPath_(path), m_Config_(std::move(config)) {}

Expected<void, errors::ProjectError> Project::Init() { return TryDeserialize(); }

Expected<void, errors::ProjectError> Project::Shutdown()
{
  if (!m_HasBeenLoaded_) {
    return {};
  }

  const auto result = TrySerialize();
  if (!result) {
    return result;
  }

  m_HasBeenLoaded_ = false;
  return {};
}

Expected<void, errors::ProjectError> Project::TrySerialize() const
{
  CORE_PROFILE_FUNCTION();

  const auto createDirectories = TryCreateProjectDirectories();
  if (!createDirectories) {
    return Unexpected(createDirectories.error());
  }

  std::ostringstream stream;

  try {
    {
      cereal::XMLOutputArchive archive(stream);
      auto config = m_Config_;
      archive(cereal::make_nvp("ProjectConfig", config));
    }
  } catch (const cereal::Exception&) {
    CORE_ASSERT(false, "Failed to serialize project config")
    return Unexpected(errors::ProjectError::ConfigSaveFailed);
  }

  const auto writeResult = Filesystem::TryWriteFile(GetConfigPath(), stream.str());
  if (!writeResult) {
    CORE_ASSERT(false, "Failed to write project config file")
    return Unexpected(errors::ProjectError::ConfigSaveFailed);
  }

  return {};
}

Expected<void, errors::ProjectError> Project::TryDeserialize()
{
  CORE_PROFILE_FUNCTION();
  if (m_HasBeenLoaded_) {
    return {};
  }

  const auto createDirectories = TryCreateProjectDirectories();
  if (!createDirectories) {
    return Unexpected(createDirectories.error());
  }

  const auto configPath = GetConfigPath();
  if (!Filesystem::FileExists(configPath)) {
    const auto serializeResult = TrySerialize();
    if (!serializeResult) {
      CORE_ASSERT(false, "Failed to create default project config file")
      return Unexpected(serializeResult.error());
    }

    m_HasBeenLoaded_ = true;
    return {};
  }

  auto configContents = Filesystem::TryReadFile(configPath);
  if (!configContents) {
    CORE_ASSERT(false, "Failed to read project config file")
    return Unexpected(errors::ProjectError::ConfigLoadFailed);
  }

  std::istringstream stream(std::move(configContents.value()));
  try {
    cereal::XMLInputArchive archive(stream);
    archive(cereal::make_nvp("ProjectConfig", m_Config_));
  } catch (const cereal::Exception&) {
    CORE_ASSERT(false, "Failed to deserialize project config")
    return Unexpected(errors::ProjectError::ConfigLoadFailed);
  }

  const auto assetRoot = GetAssetRootPath();
  if (!assetRoot.IsValid()) {
    return Unexpected(errors::ProjectError::InvalidPath);
  }

  m_Config_.AssetDirectory  = assetRoot.string();

  const auto assetDirectory = Filesystem::TryCreateDirs(assetRoot);
  if (!assetDirectory) {
    return Unexpected(errors::ProjectError::DirectoryCreationFailed);
  }

  m_HasBeenLoaded_ = true;
  return {};
}

const ProjectConfig& Project::GetConfig() const { return m_Config_; }

const EnginePath::Path& Project::GetProjectPath() const { return m_ProjectPath_; }

EnginePath::Path Project::GetAssetRootPath() const
{
  EnginePath::Path assetPath{m_Config_.AssetDirectory};
  if (assetPath.IsValid() || m_Config_.AssetDirectory.find("://") != std::string::npos) {
    return assetPath;
  }

  return EnginePath::Path{"proj://" + m_Config_.AssetDirectory};
}

EnginePath::Path Project::GetConfigPath() const
{
  EnginePath::Path config_path = m_ProjectPath_;
  config_path.append(s_ConfigFileName_);
  return config_path;
}

Expected<void, errors::ProjectError> Project::TryCreateProjectDirectories() const
{
  if (!m_ProjectPath_.IsValid()) {
    return Unexpected(errors::ProjectError::InvalidPath);
  }

  const auto rootResult = Filesystem::TryCreateDirs(m_ProjectPath_);
  if (!rootResult) {
    return Unexpected(errors::ProjectError::DirectoryCreationFailed);
  }

  const auto assetRoot = GetAssetRootPath();
  if (!assetRoot.IsValid()) {
    return Unexpected(errors::ProjectError::InvalidPath);
  }

  const auto assetResult = Filesystem::TryCreateDirs(assetRoot);
  if (!assetResult) {
    return Unexpected(errors::ProjectError::DirectoryCreationFailed);
  }

  return {};
}
} // namespace psych
