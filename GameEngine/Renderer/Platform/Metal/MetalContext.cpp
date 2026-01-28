#include "MetalContext.h"
#include "Debug/Assert.h"

namespace ge {
    MetalContext::MetalContext(GLFWwindow* window) : m_WindowHandle(window) {
        CORE_ASSERT(window, "Window is null!");
    }

    void MetalContext::Init() {
        // Implementation for initializing Metal context
    }

    void MetalContext::SwapBuffers() {
        // Implementation for swapping buffers in Metal context
    }
} // namespace ge
