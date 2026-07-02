#pragma once

#include "Errors/Errors.h"
#include "FileSystem/FileSystem.h"
#include "Project/DefaultProjectConfig.h"
#include "ge_expected"

#include <filesystem>

namespace ge
{
class Project
{
public:
  Project() = default;
  explicit Project(util::FilePath rootPath);
  Project(util::FilePath rootPath, ProjectConfig config);
  Project(Project&&)                 = default;
  Project(const Project&)            = default;
  Project& operator=(Project&&)      = default;
  Project& operator=(const Project&) = default;
  ~Project()                         = default;

  [[nodiscard]] Expected<void, errors::ProjectError> Init();
  [[nodiscard]] Expected<void, errors::ProjectError> Shutdown();
  [[nodiscard]] Expected<void, errors::ProjectError> TrySerialize() const;
  [[nodiscard]] Expected<void, errors::ProjectError> TryDeserialize();

  [[nodiscard]] const ProjectConfig& GetConfig() const;
  [[nodiscard]] const util::FilePath& GetRootPath() const;
  [[nodiscard]] util::FilePath GetAssetRootPath() const;

private:
  [[nodiscard]] util::FilePath GetConfigPath() const;
  [[nodiscard]] Expected<void, errors::ProjectError> TryCreateProjectDirectories() const;

private:
  util::FilePath m_RootPath_;
  ProjectConfig m_Config_;
  bool m_HasBeenLoaded_                                      = false;
  static inline const std::filesystem::path s_ConfigFileName_ = "project.xml";
};
} // namespace ge
