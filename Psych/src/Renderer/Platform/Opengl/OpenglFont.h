#pragma once

#include "../../Font.h"

namespace psych
{
class OpenglFont : public Font
{
public:
  OpenglFont();
  ~OpenglFont() override;

  const std::string& GetName() const override;
};
} // namespace psych
