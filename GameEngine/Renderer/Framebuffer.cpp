#include "Framebuffer.h"
#include "Renderer/Platform/Opengl/OpenglFramebuffer.h"

namespace ge
{
Shared<Framebuffer> Framebuffer::Create(const uint32_t width,
                                        const uint32_t height)
{
  return CreateShared<OpenglFramebuffer>(width, height);
}
} // namespace ge
