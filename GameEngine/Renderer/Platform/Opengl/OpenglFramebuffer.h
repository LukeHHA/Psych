#pragma once

#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"
#include "Renderer/Framebuffer.h"
#include <cstdint>

namespace ge
{
class OpenglFramebuffer : public Framebuffer
{
public:
  OpenglFramebuffer(uint32_t width, uint32_t height);
  virtual ~OpenglFramebuffer() override;

  virtual void Bind() const override;
  virtual void Unbind() const override;
  virtual void Resize(const uint32_t width, const uint32_t depth) override;
  virtual uint32_t GetWidth() const override { return m_Width; }
  virtual uint32_t GetHeight() const override { return m_Height; }
  virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const override;

private:
  static const char* FramebufferStatusToString(GLenum status)
  {
    switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
      return "GL_FRAMEBUFFER_COMPLETE";
    case GL_FRAMEBUFFER_UNDEFINED:
      return "GL_FRAMEBUFFER_UNDEFINED";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
    case GL_FRAMEBUFFER_UNSUPPORTED:
      return "GL_FRAMEBUFFER_UNSUPPORTED";
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
      return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
      return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
    case 0:
      return "glCheckFramebufferStatus returned 0";
    default:
      return "Unknown framebuffer status";
    }
  }

  static void CheckFramebufferComplete()
  {
    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status == GL_FRAMEBUFFER_COMPLETE) {
      return;
    }
    CORE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE,
                "OpenGL framebuffer is incomplete: {} ({:#x})",
                FramebufferStatusToString(status),
                status);
  }

  virtual void Invalidate();

private:
  uint32_t m_RendererID        = 0;
  uint32_t m_ColorAttachmentID = 0;
  uint32_t m_DepthAttachmentID = 0;
  uint32_t m_Width             = 0;
  uint32_t m_Height            = 0;
};
} // namespace ge
