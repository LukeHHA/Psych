#pragma once

#include "Core/Core.h"
#include <cstdint>

namespace ge
{
class Framebuffer
{
public:
  Framebuffer()                                                    = default;
  virtual ~Framebuffer()                                           = default;

  virtual void Bind() const                                        = 0;
  virtual void Unbind() const                                      = 0;
  virtual void Resize(const uint32_t width, const uint32_t height) = 0;
  virtual uint32_t GetWidth() const                                = 0;
  virtual uint32_t GetHeight() const                               = 0;
  virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const  = 0;

  static Shared<Framebuffer> Create(const uint32_t width,
                                    const uint32_t height);
};
} // namespace ge
