 
/**************************************************************************/
/*  ProjectManager.h                                                      */                                                            
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

#pragma once

#include "Core/Core.h"
#include "Errors/Errors.h"
#include "FileSystem/PathResolver.h"
#include "Project/Project.h"
#include "expected.h"

#include <filesystem>

namespace psych
{
class ProjectManager
{
public:
  ProjectManager()                                 = default;
  ProjectManager(const ProjectManager&)            = delete;
  ProjectManager& operator=(const ProjectManager&) = delete;
  ProjectManager(ProjectManager&&)                 = delete;
  ProjectManager& operator=(ProjectManager&&)      = delete;
  ~ProjectManager()                                = default;

  [[nodiscard]] Expected<void, errors::ProjectError> Init(PathResolver& resolver);
  [[nodiscard]] Expected<void, errors::ProjectError> Shutdown(PathResolver& resolver);
  [[nodiscard]] Expected<void, errors::ProjectError> OpenProject(std::filesystem::path projectRoot, PathResolver& resolver);

  [[nodiscard]] bool HasActiveProject() const;
  [[nodiscard]] Project& GetActiveProject();
  [[nodiscard]] const Project& GetActiveProject() const;

private:
  Unique<Project> m_ActiveProject_;
};
} // namespace psych
