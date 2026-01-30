#pragma once

#include "Core/Base.h"
#include "Renderer/RendererContext.h"
#include "GLFW/glfw3.h"
#include "ge_expected"
#include "Errors/Errors.h"
#include "Core/Event.h"

#include <string>
#include <functional>

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
        using EventCallbackFn = std::function<void(Event&)>;

        Window() = default;
        virtual ~Window() = default;

        virtual util::expected<void, errors::WindowError> Init(const std::string& title, unsigned int width, unsigned int height) = 0;
        virtual util::expected<void, errors::WindowError> Shutdown() = 0;
        virtual void OnUpdate() = 0;
        unsigned int GetWidth() const { return m_Data.Width; }
        unsigned int GetHeight() const { return m_Data.Height; }
        GLFWwindow* GetNativeWindow() const { return m_Window.get(); }
        virtual void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;
        static Shared<Window> Create(const std::string& title, unsigned int width, unsigned int height);

    private:
        Shared<RendererContext> m_RendererContext;
        UniqueGLFWwindow m_Window;

        struct WindowData {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

} // namespace ge