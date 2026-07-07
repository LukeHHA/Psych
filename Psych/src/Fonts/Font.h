#pragma once

#include <string>

namespace psych
{
class Font
{
public:
  Font(const Font&)            = default;
  Font(Font&&)                 = delete;
  Font& operator=(const Font&) = default;
  Font& operator=(Font&&)      = delete;
  virtual ~Font();

  [[nodiscard]] virtual const std::string& GetName() const = 0;
};
} // namespace psych
