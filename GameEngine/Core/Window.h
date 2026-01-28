#pragma once

#include "Core/Base.h"
#include "Renderer/RendererContext.h"
#include "GLFW/glfw3.h"
#include "ge_expected"

#include <string>

namespace ge {

    struct GLFWwindowDeleter {
        void operator()(GLFWwindow* window) const noexcept {
            // Custom deleter logic for GLFWwindow
            if (window) {
                // Assume glfwDestroyWindow is the function to destroy a
                // GLFWwindow
                glfwDestroyWindow(window);
            }
        }
    };

    using UniqueGLFWwindow = Unique<GLFWwindow, GLFWwindowDeleter>;

    class Window {
    public:
        Window(const std::string& title, unsigned int width, unsigned int height);
        ~Window() = default;

        util::expected<bool, std::string> Init(const std::string& title, unsigned int width, unsigned int height);

        void OnUpdate();
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
        GLFWwindow* GetNativeWindow() const;

        static Shared<Window> Create(const std::string& title, unsigned int width, unsigned int height);

    private:
        Shared<RendererContext> m_RendererContext;
        UniqueGLFWwindow m_Window;
    };

} // namespace ge