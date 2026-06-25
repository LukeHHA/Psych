#include "FontManager.h"
#include "Debug/Assert.h"
#include "Errors/Errors.h"

#include <utility>

namespace ge
{
void FontManager::RegisterFont(std::string name, BinaryBlob font)
{
  const auto result = TryRegisterFont(std::move(name), font);
  if (!result) {
    CORE_ASSERT(false, "Failed to add font to library")
  }
}

Expected<void, errors::FilesystemError> FontManager::TryRegisterFont(std::string name, BinaryBlob font)
{
  if (name.empty() || !font.IsValid()) {
    return Unexpected(errors::FilesystemError::LoadFailed);
  }

  auto [it, res] = m_FontLibrary_.emplace(std::move(name), font);
  if (!res) {
    return Unexpected(errors::FilesystemError::LoadFailed);
  }

  return {};
}

const BinaryBlob& FontManager::GetFontFromLibrary(const std::string& key) const
{
  CORE_ASSERT(m_FontLibrary_.contains(key), "Font does not exist in library: {}", key)
  return m_FontLibrary_.at(key);
}

} // namespace ge
