#pragma once

#include "Renderer/RendererContext.h"

namespace ge {
    class OpenglContext : public RendererContext {
    public:
        OpenglContext(GLFWwindow* window);
        virtual ~OpenglContext() = default;

        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
} // namespace ge
