#pragma once

#include "Assets/AssetHandle.h"
#include "Errors/Errors.h"
#include "Resources/BinaryBuffer.h"
#include "ge_expected"

#include <string>
#include <unordered_map>

namespace ge
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

  void RegisterFont(std::string name, BinaryBuffer font);
  Expected<void, errors::FilesystemError> TryRegisterFont(std::string name, BinaryBuffer font);

  [[nodiscard]] const BinaryBuffer& GetFontFromLibrary(const std::string& key) const;
  [[nodiscard]] const BinaryBuffer& GetDefaultFont();

private:
  BinaryBuffer m_DefaultFont_;
  std::unordered_map<AssetHandle, BinaryBuffer> m_AssetFontLibrary_;
  std::unordered_map<std::string, BinaryBuffer> m_FontLibrary_;
};

} // namespace ge
