#pragma once

#include "Core/Core.h"
#include "Errors/Errors.h"
#include "FileSystem/PathResolver.h"
#include "Project/Project.h"
#include "ge_expected"

namespace ge
{
class ProjectManager
{
public:
  ProjectManager()                                  = default;
  ProjectManager(const ProjectManager&)            = delete;
  ProjectManager& operator=(const ProjectManager&) = delete;
  ProjectManager(ProjectManager&&)                 = delete;
  ProjectManager& operator=(ProjectManager&&)      = delete;
  ~ProjectManager()                                = default;

  [[nodiscard]] Expected<void, errors::ProjectError> Init(util::PathResolver& resolver);
  [[nodiscard]] Expected<void, errors::ProjectError> Shutdown(util::PathResolver& resolver);
  [[nodiscard]] Expected<void, errors::ProjectError> OpenProject(util::FilePath projectRoot, util::PathResolver& resolver);

  [[nodiscard]] bool HasActiveProject() const;
  [[nodiscard]] Project& GetActiveProject();
  [[nodiscard]] const Project& GetActiveProject() const;

private:
  Unique<Project> m_ActiveProject_;
};
} // namespace ge
