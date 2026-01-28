#include "OpenglContext.h"
#include "Debug/Assert.h"
#include <Debug/Instrumentor.h>

namespace ge {
    OpenglContext::OpenglContext(GLFWwindow* window) : m_WindowHandle(window) {
        CORE_PROFILE_FUNCTION();
        CORE_ASSERT(window, "Window handle is null!");
    }
} // namespace ge
