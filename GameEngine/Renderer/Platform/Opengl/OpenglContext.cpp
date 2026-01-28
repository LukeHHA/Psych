#include "OpenglContext.h"
#include "Debug/Assert.h"

namespace ge {
    OpenglContext::OpenglContext(GLFWwindow* window) : m_WindowHandle(window) {
        CORE_ASSERT(window, "Window handle is null!");
    }
} // namespace ge
