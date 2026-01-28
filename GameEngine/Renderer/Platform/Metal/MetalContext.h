#pragma once

#include "Renderer/RendererContext.h"

namespace ge {
    class MetalContext : public RendererContext {
    public:
        MetalContext(GLFWwindow* window);
        virtual ~MetalContext() = default;

        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
} // namespace ge
