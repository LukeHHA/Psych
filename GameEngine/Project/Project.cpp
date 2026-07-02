#include "Project.h"

#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "cereal/archives/xml.hpp"

#include <fstream>
#include <utility>

namespace ge
{
Project::Project(util::FilePath rootPath) : m_RootPath_(std::move(rootPath)) {}

Project::Project(util::FilePath rootPath, ProjectConfig config) : m_RootPath_(std::move(rootPath)), m_Config_(std::move(config)) {}

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

  std::ofstream stream(GetConfigPath());
  if (!stream) {
    CORE_ASSERT(false, "Failed to open project config file for writing")
    return Unexpected(errors::ProjectError::ConfigSaveFailed);
  }

  try {
    cereal::XMLOutputArchive archive(stream);
    auto config = m_Config_;
    archive(cereal::make_nvp("ProjectConfig", config));
  } catch (const cereal::Exception&) {
    CORE_ASSERT(false, "Failed to serialize project config")
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
  if (!util::Filesystem::FileExists(configPath)) {
    const auto serializeResult = TrySerialize();
    if (!serializeResult) {
      CORE_ASSERT(false, "Failed to create default project config file")
      return Unexpected(serializeResult.error());
    }

    m_HasBeenLoaded_ = true;
    return {};
  }

  std::ifstream stream(configPath);
  if (!stream) {
    CORE_ASSERT(false, "Failed to open project config file for reading")
    return Unexpected(errors::ProjectError::ConfigLoadFailed);
  }

  try {
    cereal::XMLInputArchive archive(stream);
    archive(cereal::make_nvp("ProjectConfig", m_Config_));
  } catch (const cereal::Exception&) {
    CORE_ASSERT(false, "Failed to deserialize project config")
    return Unexpected(errors::ProjectError::ConfigLoadFailed);
  }

  const auto assetDirectory = util::Filesystem::TryCreateDirs(GetAssetRootPath());
  if (!assetDirectory) {
    return Unexpected(errors::ProjectError::DirectoryCreationFailed);
  }

  m_HasBeenLoaded_ = true;
  return {};
}

const ProjectConfig& Project::GetConfig() const { return m_Config_; }

const util::FilePath& Project::GetRootPath() const { return m_RootPath_; }

util::FilePath Project::GetAssetRootPath() const
{
  const util::FilePath assetDirectory = m_Config_.AssetDirectory;
  if (assetDirectory.is_absolute()) {
    return assetDirectory;
  }

  return m_RootPath_ / assetDirectory;
}

util::FilePath Project::GetConfigPath() const { return m_RootPath_ / s_ConfigFileName_; }

Expected<void, errors::ProjectError> Project::TryCreateProjectDirectories() const
{
  if (m_RootPath_.empty()) {
    return Unexpected(errors::ProjectError::InvalidPath);
  }

  const auto rootResult = util::Filesystem::TryCreateDirs(m_RootPath_);
  if (!rootResult) {
    return Unexpected(errors::ProjectError::DirectoryCreationFailed);
  }

  const auto assetResult = util::Filesystem::TryCreateDirs(GetAssetRootPath());
  if (!assetResult) {
    return Unexpected(errors::ProjectError::DirectoryCreationFailed);
  }

  return {};
}
} // namespace ge
