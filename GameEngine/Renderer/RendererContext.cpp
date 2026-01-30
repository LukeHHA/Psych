#include "Renderer/RendererContext.h"
#include "Renderer/Platform/Opengl/OpenglContext.h"
#include "Renderer/Platform/Vulkan/VulkanContext.h"
#include "Renderer/RendererAPI.h"
#include <Debug/Instrumentor.h>

namespace ge {
    Shared<RendererContext> RendererContext::Create(GLFWwindow* window) {
        CORE_PROFILE_FUNCTION();
        switch (RendererAPI::Current()) {
            case RendererAPIType::NONE:
                return nullptr;
            case RendererAPIType::TEST_HEADLESS:
                return CreateShared<RendererContextHeadless>();
            case RendererAPIType::OPENGL:
                return CreateShared<OpenglContext>(window);
            case RendererAPIType::VULKAN:
                return CreateShared<VulkanContext>(window);
            case RendererAPIType::METAL:
                return CreateShared<OpenglContext>(window);
            default:
                CORE_ASSERT(false, "Unknown Context during rendererContext::Create()")
                return nullptr;
        }
    }
} // namespace ge
