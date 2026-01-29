#pragma once

#include "Renderer/RendererContext.h"
#include "Renderer/RendererAPI.h"

namespace ge {
    class OpenglContext : public RendererContext {
    public:
        OpenglContext(GLFWwindow* window);
        virtual ~OpenglContext() = default;

        virtual void Init() override;
        virtual void SwapBuffers() override;
        virtual RendererAPIType GetCurrentAPI() const override { return RendererAPIType::Opengl; }

    private:
        GLFWwindow* m_WindowHandle;
        RendererAPIType m_API = RendererAPIType::Opengl;
    };
} // namespace ge
