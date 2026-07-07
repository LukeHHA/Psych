#pragma once

#include "Assets/AssetHandle.h"
#include "Errors/Errors.h"
#include "Resources/BinaryBuffer.h"
#include "expected.h"

#include <filesystem>

namespace psych
{
class FontManager
{
public:
  FontManager()                              = default;
  FontManager(FontManager&&)                 = default;
  FontManager(const FontManager&)            = default;
  FontManager& operator=(FontManager&&)      = default;
  FontManager& operator=(const FontManager&) = default;
  ~FontManager()                             = default;

  void RegisterMemoryFont(const std::string& name, BinaryBuffer& font);
  Expected<void, errors::FilesystemError> TryRegisterMemoryFont(const std::string& name, BinaryBuffer& font);

  void RegistryFileFont(const std::string& name, const std::filesystem::path& path);
  void TryRegistryFileFont(const std::string& name, const std::filesystem::path& path);

  [[nodiscard]] const BinaryBuffer& GetFontFromLibrary(const std::string& key) const;
  [[nodiscard]] const BinaryBuffer& GetDefaultFont();

private:
  BinaryBuffer m_DefaultFont_;
  std::unordered_map<AssetHandle, BinaryBuffer> m_AssetFontLibrary_;
  std::unordered_map<std::string, BinaryBuffer> m_FontLibrary_;
};

} // namespace psych
