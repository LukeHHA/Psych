#include "VulkanContext.h"
#include "Debug/Assert.h"

namespace ge {
    // VulkanContext::VulkanContext(GLFWwindow* window) : m_WindowHandle(window) {
    //     CORE_ASSERT(window, "Window is null!");
    // }

    VulkanContext::VulkanContext() = default;

    void VulkanContext::Init() {
        // Vulkan-specific initialization code would go here
    }
    void VulkanContext::SwapBuffers() {
        // Vulkan does not use traditional buffer swapping like OpenGL.
        // Presentation is handled differently, so this function may be empty
        // or contain Vulkan-specific presentation logic.
    }
} // namespace ge
