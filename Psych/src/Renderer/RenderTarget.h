 
/**************************************************************************/
/*  RenderTarget.h                                                        */                                                            
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

#include "Core/Window.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"
#include "Renderer/Framebuffer.h"
#include <cstdint>
#include <utility>

namespace psych
{
class RenderTarget
{
public:
  RenderTarget()          = default;
  virtual ~RenderTarget() = default;
  CORE_NO_COPY_NO_MOVE(RenderTarget);

  virtual void Bind()                                  = 0;
  virtual void Unbind()                                = 0;
  [[nodiscard]] virtual uint32_t GetWidth() const      = 0;
  [[nodiscard]] virtual uint32_t GetHeight() const     = 0;
  virtual void Resize(uint32_t width, uint32_t height) = 0;
};

class FramebufferRenderTarget : public RenderTarget
{
public:
  explicit FramebufferRenderTarget(Framebuffer& framebuffer) : m_Framebuffer_(framebuffer) {}

  void Bind() override { m_Framebuffer_.Bind(); }
  void Unbind() override { m_Framebuffer_.Unbind(); }
  [[nodiscard]] uint32_t GetWidth() const override { return m_Framebuffer_.GetWidth(); }
  [[nodiscard]] uint32_t GetHeight() const override { return m_Framebuffer_.GetHeight(); }
  void Resize(uint32_t width, uint32_t height) override { m_Framebuffer_.Resize(width, height); }

private:
  Framebuffer& m_Framebuffer_;
};

class WindowRenderTarget : public RenderTarget
{
public:
  explicit WindowRenderTarget(Window& window) : m_Window_(window) {}

  void Bind() override
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, GetWidth(), GetHeight());
  }
  void Unbind() override {}
  [[nodiscard]] uint32_t GetWidth() const override { return m_Window_.GetWidth(); }
  [[nodiscard]] uint32_t GetHeight() const override { return m_Window_.GetHeight(); }
  void Resize(uint32_t width, uint32_t height) override {}

private:
  Window& m_Window_;
};
} // namespace psych
