#pragma once

#include "Core/Base.h"

struct GLFWwindow;
namespace ge {

    class RendererContext {
    public:
        RendererContext() = default;
        virtual ~RendererContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

        static Shared<RendererContext> Create(GLFWwindow* window);
    };
} // namespace ge
