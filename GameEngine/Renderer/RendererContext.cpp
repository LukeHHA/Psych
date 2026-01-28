#include "Renderer/RendererContext.h"
#include "Renderer/Platform/Opengl/OpenglContext.h"
#include "Renderer/Platform/Vulkan/VulkanContext.h"
#include "Renderer/RendererAPI.h"

namespace ge {
    Shared<RendererContext> RendererContext::Create(GLFWwindow* window) {
        switch (RendererAPI::Current()) {
        case RendererAPIType::None:
            return nullptr;
        case RendererAPIType::Opengl:
            return CreateShared<OpenglContext>(window);
        case RendererAPIType::Vulkan:
            return CreateShared<VulkanContext>(window);
        case RendererAPIType::Metal:
            return CreateShared<OpenglContext>(window);
        default:
            return nullptr;
        }
    }
} // namespace ge
