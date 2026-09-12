 
/**************************************************************************/
/*  Framebuffer.h                                                         */                                                            
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PSYCH ENGINE                               */
/**************************************************************************/
/* Copyright (c)  Luke Howe                                               */                                                  
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "Core/Core.h"
#include <cstdint>

namespace psych
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
} // namespace psych
