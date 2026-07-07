#include "MetalContext.h"
#include "Debug/Assert.h"

namespace psych
{
// MetalContext::MetalContext(GLFWwindow* window) : m_WindowHandle(window) {
//     CORE_ASSERT(window, "Window is null!");
// }

MetalContext::MetalContext() = default;

Expected<void, errors::RendererError> MetalContext::Init()
{
  // Implementation for initializing Metal context
  return {};
}

void MetalContext::SwapBuffers()
{
  // Implementation for swapping buffers in Metal context
}
} // namespace psych
