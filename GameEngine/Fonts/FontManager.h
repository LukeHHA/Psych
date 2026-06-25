#pragma once

#include "Errors/Errors.h"
#include "Resources/BinaryBlob.h"
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

  void RegisterFont(std::string name, BinaryBlob font);
  Expected<void, errors::FilesystemError> TryRegisterFont(std::string name, BinaryBlob font);

  [[nodiscard]] const BinaryBlob& GetFontFromLibrary(const std::string& key) const;

private:
  std::unordered_map<std::string, BinaryBlob> m_FontLibrary_;
};

} // namespace ge
