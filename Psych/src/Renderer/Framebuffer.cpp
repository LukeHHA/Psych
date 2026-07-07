#include "Framebuffer.h"
#include "Renderer/Platform/Opengl/OpenglFramebuffer.h"

namespace psych
{
Unique<Framebuffer> Framebuffer::Create(const uint32_t width, const uint32_t height) { return CreateUnique<OpenglFramebuffer>(width, height); }
} // namespace psych
