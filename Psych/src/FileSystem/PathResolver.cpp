
/**************************************************************************/
/*  PathResolver.cpp                                                      */
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

#include "PathResolver.h"
#include "FileSystem/FileSystem.h"
#include <filesystem>

namespace psych
{
Expected<std::filesystem::path, errors::FilesystemError> PathResolver::TryResolve(const EnginePath::Path& path) const
{
  if (!path.IsValid()) {
    return Unexpected(errors::FilesystemError::InvalidPath);
  }

  const auto& relativePath = path.GetRelativePath();

  switch (path.GetSchema()) {
  case EnginePath::Schema::Config: {
    auto baseConfigPath = Filesystem::TryGetBaseConfigPath();
    if (!baseConfigPath) {
      return Unexpected(errors::FilesystemError::OSPathFail);
    }
    return baseConfigPath.value() / relativePath;
  }

  case EnginePath::Schema::Engine:
    if (m_EngineRoot_.empty()) {
      return Unexpected(errors::FilesystemError::InvalidPath);
    }
    return m_EngineRoot_ / relativePath;

  case EnginePath::Schema::Cache: {
    auto baseCachePath = Filesystem::TryGetBaseCachePath();
    if (!baseCachePath) {
      return Unexpected(errors::FilesystemError::OSPathFail);
    }
    return baseCachePath.value() / relativePath;
  }

  case EnginePath::Schema::None:
    break;
  }

  return Unexpected(errors::FilesystemError::InvalidPath);
}
} // namespace psych
