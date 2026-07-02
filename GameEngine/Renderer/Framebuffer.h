#pragma once

#include "Core/Core.h"
#include <cstdint>

namespace ge
{
class Framebuffer
{
public:
  Framebuffer()                                                                 = default;
  Framebuffer(const Framebuffer&)                                               = default;
  Framebuffer(Framebuffer&&)                                                    = delete;
  Framebuffer& operator=(const Framebuffer&)                                    = default;
  Framebuffer& operator=(Framebuffer&&)                                         = delete;
  virtual ~Framebuffer()                                                        = default;

  virtual void Bind() const                                                     = 0;
  virtual void Unbind() const                                                   = 0;
  virtual void Resize(const uint32_t width, const uint32_t height)              = 0;
  [[nodiscard]] virtual uint32_t GetWidth() const                               = 0;
  [[nodiscard]] virtual uint32_t GetHeight() const                              = 0;
  [[nodiscard]] virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const = 0;

  static Unique<Framebuffer> Create(const uint32_t width, const uint32_t height);
};
} // namespace ge
