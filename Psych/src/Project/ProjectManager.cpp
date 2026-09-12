
/**************************************************************************/
/*  ProjectManager.cpp                                                    */
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

#include "ProjectManager.h"

#include "Core/PsychEngine.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "FileSystem/FileSystem.h"

#include <algorithm>
#include <utility>

namespace psych
{
Expected<void, errors::ProjectError> ProjectManager::Init()
{
  CORE_PROFILE_FUNCTION();
  return {};
}

Expected<void, errors::ProjectError> ProjectManager::Shutdown()
{
  CORE_PROFILE_FUNCTION();

  if (!m_ActiveProject_) {
    Filesystem::ClearProjectRoot();
    m_ActiveProjectRoot_.clear();
    return {};
  }

  const auto result = m_ActiveProject_->Shutdown();
  if (!result) {
    return result;
  }

  m_ActiveProject_.reset();
  m_ActiveProjectRoot_.clear();
  Filesystem::ClearProjectRoot();
  return {};
}

Expected<void, errors::ProjectError> ProjectManager::OpenProject(const std::filesystem::path& path)
{
  CORE_PROFILE_FUNCTION();

  if (path.empty()) {
    return Unexpected(errors::ProjectError::InvalidPath);
  }

  const auto projectRoot = path.lexically_normal();
  const auto bindResult  = Filesystem::TrySetProjectRoot(projectRoot);

  if (!bindResult) {
    return Unexpected(errors::ProjectError::InvalidPath);
  }

  auto project = CreateUnique<Project>(EnginePath::Path{"proj://"});
  auto result  = project->Init();
  if (!result) {
    if (m_ActiveProject_) {
      const auto restoreResult = Filesystem::TrySetProjectRoot(m_ActiveProjectRoot_);
      CORE_ASSERT(restoreResult, "Failed to restore active project root")
    } else {
      Filesystem::ClearProjectRoot();
    }
    return result;
  }

  if (m_ActiveProject_) {
    const auto restoreResult = Filesystem::TrySetProjectRoot(m_ActiveProjectRoot_);
    if (!restoreResult) {
      return Unexpected(errors::ProjectError::InvalidPath);
    }

    const auto shutdownResult = m_ActiveProject_->Shutdown();
    if (!shutdownResult) {
      return shutdownResult;
    }

    const auto rebindResult = Filesystem::TrySetProjectRoot(projectRoot);
    if (!rebindResult) {
      return Unexpected(errors::ProjectError::InvalidPath);
    }
  }

  // updates previous projects config value with the 5 most recent projects
  auto& previous_projects = PsychEngine::Get().GetConfig().Projects.PreviousProjects;
  if (previous_projects.size() < 5) {
    if (auto it = std::ranges::find(previous_projects.begin(), previous_projects.end(), projectRoot.string()); it == previous_projects.end()) {
      previous_projects.push_back(projectRoot);
    }
  }

  m_ActiveProject_     = std::move(project);
  m_ActiveProjectRoot_ = projectRoot;
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
