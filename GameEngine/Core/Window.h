#pragma once

#include "Core/Base.h"
#include "Renderer/RendererContext.h"
#include "GLFW/glfw3.h"
#include "ge_expected"
#include "Errors/Errors.h"
#include "Events/Event.h"

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
        Window(const Window& other) = delete;
        Window(Window&& other) = delete;
        Window& operator=(const Window& other) = delete;
        Window& operator=(Window&& other) = delete;

        virtual util::expected<void, errors::WindowError> Init(const std::string& title, unsigned int width, unsigned int height) = 0;
        virtual util::expected<void, errors::WindowError> Shutdown() = 0;
        virtual void OnUpdate() = 0;
        virtual void PollEvents() = 0;
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        virtual GLFWwindow* GetNativeWindow() const = 0;
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
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