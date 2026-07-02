#include "FontManager.h"
#include "Debug/Assert.h"
#include "EmbeddedResources.h"
#include "Errors/Errors.h"
#include "Resources/BinaryBuffer.h"

namespace ge
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

} // namespace ge
