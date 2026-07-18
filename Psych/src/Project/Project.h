 
/**************************************************************************/
/*  Project.h                                                             */                                                            
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

#include "Errors/Errors.h"
#include "FileSystem/FileSystem.h"
#include "Project/DefaultProjectConfig.h"
#include "expected.h"

#include <filesystem>

namespace psych
{
class Project
{
public:
  Project() = default;
  explicit Project(std::filesystem::path rootPath);
  Project(std::filesystem::path rootPath, ProjectConfig config);
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
  [[nodiscard]] const std::filesystem::path& GetRootPath() const;
  [[nodiscard]] std::filesystem::path GetAssetRootPath() const;

private:
  [[nodiscard]] std::filesystem::path GetConfigPath() const;
  [[nodiscard]] Expected<void, errors::ProjectError> TryCreateProjectDirectories() const;

private:
  std::filesystem::path m_RootPath_;
  ProjectConfig m_Config_;
  bool m_HasBeenLoaded_                                       = false;
  static inline const std::filesystem::path s_ConfigFileName_ = "project.xml";
};
} // namespace psych
