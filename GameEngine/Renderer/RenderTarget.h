#pragma once

#include "Core/Window.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"
#include "Renderer/Framebuffer.h"
#include <cstdint>
#include <utility>

namespace ge
{
class RenderTarget
{
public:
  RenderTarget()          = default;
  virtual ~RenderTarget() = default;
  CORE_NO_COPY_NO_MOVE(RenderTarget);

  virtual void Bind()                                  = 0;
  virtual void Unbind()                                = 0;
  virtual uint32_t GetWidth() const                    = 0;
  virtual uint32_t GetHeight() const                   = 0;
  virtual void Resize(uint32_t width, uint32_t height) = 0;
};

class FramebufferRenderTarget : public RenderTarget
{
public:
  explicit FramebufferRenderTarget(Shared<Framebuffer> framebuffer)
      : m_Framebuffer_(std::move(framebuffer))
  {
    CORE_ASSERT(m_Framebuffer_,
                "FramebufferRenderTarget requires a valid framebuffer")
  }

  virtual void Bind() override { m_Framebuffer_->Bind(); }
  virtual void Unbind() override { m_Framebuffer_->Unbind(); }
  virtual uint32_t GetWidth() const override
  {
    return m_Framebuffer_->GetWidth();
  }
  virtual uint32_t GetHeight() const override
  {
    return m_Framebuffer_->GetHeight();
  }
  virtual void Resize(uint32_t width, uint32_t height) override
  {
    m_Framebuffer_->Resize(width, height);
  }

private:
  Shared<Framebuffer> m_Framebuffer_;
};

class WindowRenderTarget : public RenderTarget
{
public:
  explicit WindowRenderTarget(Window& window) : m_Window_(window) {}

  virtual void Bind() override
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, GetWidth(), GetHeight());
  }
  virtual void Unbind() override {}
  virtual uint32_t GetWidth() const override { return m_Window_.GetWidth(); }
  virtual uint32_t GetHeight() const override { return m_Window_.GetHeight(); }
  virtual void Resize(uint32_t, uint32_t) override {}

private:
  Window& m_Window_;
};
} // namespace ge
