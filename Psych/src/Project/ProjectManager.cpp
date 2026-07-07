#include "ProjectManager.h"

#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"

#include <filesystem>
#include <utility>

namespace psych
{
Expected<void, errors::ProjectError> ProjectManager::Init(util::PathResolver& resolver)
{
  CORE_PROFILE_FUNCTION();

  std::error_code ec;
  auto projectRoot = std::filesystem::current_path(ec);
  if (ec) {
    return Unexpected(errors::ProjectError::InvalidPath);
  }

  return OpenProject(std::move(projectRoot), resolver);
}

Expected<void, errors::ProjectError> ProjectManager::Shutdown(util::PathResolver& resolver)
{
  CORE_PROFILE_FUNCTION();

  if (!m_ActiveProject_) {
    return {};
  }

  const auto result = m_ActiveProject_->Shutdown();
  if (!result) {
    return result;
  }

  m_ActiveProject_.reset();
  return {};
}

Expected<void, errors::ProjectError> ProjectManager::OpenProject(util::FilePath projectRoot, util::PathResolver& resolver)
{
  CORE_PROFILE_FUNCTION();

  auto project = CreateUnique<Project>(std::move(projectRoot));
  auto result  = project->Init();
  if (!result) {
    return result;
  }

  if (m_ActiveProject_) {
    const auto shutdownResult = m_ActiveProject_->Shutdown();
    if (!shutdownResult) {
      return shutdownResult;
    }
  }

  m_ActiveProject_ = std::move(project);
  return {};
}

bool ProjectManager::HasActiveProject() const { return m_ActiveProject_ != nullptr; }

Project& ProjectManager::GetActiveProject()
{
  CORE_ASSERT(m_ActiveProject_, "No active project")
  return *m_ActiveProject_;
}

const Project& ProjectManager::GetActiveProject() const
{
  CORE_ASSERT(m_ActiveProject_, "No active project")
  return *m_ActiveProject_;
}
} // namespace psych
