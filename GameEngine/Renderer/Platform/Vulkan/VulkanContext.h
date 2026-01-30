#pragma once

#include "Renderer/RendererContext.h"
#include "Renderer/RendererAPI.h"

namespace ge {
    class VulkanContext : public RendererContext {
    public:
        VulkanContext(GLFWwindow* window);
        virtual ~VulkanContext() = default;

        virtual void Init() override;
        virtual void SwapBuffers() override;
        virtual RendererAPIType GetCurrentAPI() const override { return RendererAPIType::VULKAN; }

    private:
        GLFWwindow* m_WindowHandle;
        RendererAPIType m_API = RendererAPIType::VULKAN;
    };
} // namespace ge
