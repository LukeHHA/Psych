#pragma once

#include "Renderer/RendererContext.h"

namespace ge {
    class VulkanContext : public RendererContext {
    public:
        VulkanContext(GLFWwindow* window);
        virtual ~VulkanContext() = default;

        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
} // namespace ge
