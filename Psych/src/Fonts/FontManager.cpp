 
/**************************************************************************/
/*  FontManager.cpp                                                       */                                                            
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

#include "FontManager.h"
#include "Debug/Assert.h"
#include "EmbeddedResources.h"
#include "Errors/Errors.h"
#include "Resources/BinaryBuffer.h"

namespace psych
{
void FontManager::RegisterMemoryFont(const std::string& name, BinaryBuffer& font)
{
  const auto result = TryRegisterMemoryFont(name, font);
  if (!result) {
    CORE_ASSERT(false, "Failed to add font to library")
  }
}

Expected<void, errors::FilesystemError> FontManager::TryRegisterMemoryFont(const std::string& name, BinaryBuffer& font)
{
  if (name.empty() || !font.IsValid()) {
    return Unexpected(errors::FilesystemError::LoadFailed);
  }

  auto [it, res] = m_FontLibrary_.emplace(name, font);
  if (!res) {
    return Unexpected(errors::FilesystemError::LoadFailed);
  }

  return {};
}

void FontManager::RegistryFileFont(const std::string& name, const std::filesystem::path& path) {}
void FontManager::TryRegistryFileFont(const std::string& name, const std::filesystem::path& path) {}

const BinaryBuffer& FontManager::GetFontFromLibrary(const std::string& key) const
{
  CORE_ASSERT(m_FontLibrary_.contains(key), "Font does not exist in library: {}", key)
  return m_FontLibrary_.at(key);
}

const BinaryBuffer& FontManager::GetDefaultFont()
{
  if (m_DefaultFont_.size() == 0) {
    m_DefaultFont_ = embedded::JetBrainsMonoNerdFontRegular();
  }
  return m_DefaultFont_;
}

} // namespace psych
