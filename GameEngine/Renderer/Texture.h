#pragma once

#include "Core/Base.h"
#include <cstdint>

namespace ge
{
class Texture
{
public:
  virtual ~Texture()                         = default;

  virtual void Bind(uint32_t slot = 0) const = 0;
  virtual uint32_t GetTextureID() const      = 0;
};

class Texture2D : public Texture
{
public:
  virtual ~Texture2D()                       = default;

  virtual void Bind(uint32_t slot = 0) const = 0;
  static Shared<Texture> Create(const std::string& path);
};
} // namespace ge
