 
/**************************************************************************/
/*  LinuxFilesystemAPI.cpp                                                */                                                            
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

#include "LinuxFilesystemAPI.h"

#include <cstdlib>

namespace psych
{
DirPath LinuxFilesystemAPI::GetOSAppDataPath() const
{
  if (const char* configHome = std::getenv("XDG_CONFIG_HOME")) {
    if (*configHome != '\0') {
      return DirPath{configHome};
    }
  }

  const auto homePath = GetHomePath();
  if (homePath.empty()) {
    return {};
  }

  return homePath / ".config";
}

DirPath LinuxFilesystemAPI::GetOSCacheDataPath() const
{
  if (const char* cacheHome = std::getenv("XDG_CACHE_HOME")) {
    if (*cacheHome != '\0') {
      return DirPath{cacheHome};
    }
  }

  const auto homePath = GetHomePath();
  if (homePath.empty()) {
    return {};
  }

  return homePath / ".cache";
}

DirPath LinuxFilesystemAPI::GetHomePath() const
{
  if (const char* home = std::getenv("HOME")) {
    if (*home != '\0') {
      return DirPath{home};
    }
  }

  return {};
}
} // namespace psych
