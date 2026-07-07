#pragma once

#include <cstdint>

#include "../../Texture.h"

namespace psych
{
class OpenglTexture2D : public Texture2D
{
public:
  OpenglTexture2D(const std::string& path);
  virtual ~OpenglTexture2D() = default;

  virtual void Bind(uint32_t slot = 0) const override;
  virtual uint32_t GetTextureID() const override { return m_TextureID_; };

private:
  uint32_t m_TextureID_ = 0;
};
} // namespace psych
