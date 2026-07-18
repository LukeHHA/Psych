 
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

#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"

#include <filesystem>
#include <utility>

namespace psych
{
Expected<void, errors::ProjectError> ProjectManager::Init(PathResolver& resolver)
{
  CORE_PROFILE_FUNCTION();

  std::error_code ec;
  auto projectRoot = std::filesystem::current_path(ec);
  if (ec) {
    return Unexpected(errors::ProjectError::InvalidPath);
  }

  return OpenProject(std::move(projectRoot), resolver);
}

Expected<void, errors::ProjectError> ProjectManager::Shutdown(PathResolver& resolver)
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

Expected<void, errors::ProjectError> ProjectManager::OpenProject(std::filesystem::path projectRoot, PathResolver& resolver)
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
