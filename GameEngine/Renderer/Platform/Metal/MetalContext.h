#pragma once

#include "Renderer/RendererContext.h"
#include "Renderer/RendererAPI.h"

namespace ge {
    class MetalContext : public RendererContext {
    public:
        MetalContext(GLFWwindow* window);
        virtual ~MetalContext() = default;

        virtual void Init() override;
        virtual void SwapBuffers() override;
        virtual RendererAPIType GetCurrentAPI() const override { return RendererAPIType::Metal; }

    private:
        GLFWwindow* m_WindowHandle;
        RendererAPIType m_API = RendererAPIType::Metal;
    };
} // namespace ge
