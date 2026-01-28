#include "VulkanContext.h"
#include "Debug/Assert.h"

namespace ge {
    VulkanContext::VulkanContext(GLFWwindow* window) : m_WindowHandle(window) {
        CORE_ASSERT(window, "Window is null!");
    }
} // namespace ge
