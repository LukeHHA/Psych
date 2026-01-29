#include "Window.h"
#include "Debug/Instrumentor.h"
#include "Debug/Assert.h"

namespace ge {

    static void GLFWErrorCallback(int error, const char* description) {
        CORE_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window::Window(const std::string& title, unsigned int width, unsigned int height) {
        CORE_PROFILE_FUNCTION();
        {
            CORE_PROFILE_SCOPE("Window::Init");
            Init(title, width, height);
        }
    }

    Window::~Window() {
        CORE_PROFILE_FUNCTION();
        {
            CORE_PROFILE_SCOPE("Window::Shutdown");
            Shutdown();
        }
    }

    util::expected<void, errors::WindowError> Window::Init(const std::string& title, unsigned int width, unsigned int height) {
        CORE_PROFILE_FUNCTION();

        if (!m_Window) {
            m_Data.Title = title;
            m_Data.Width = width;
            m_Data.Height = height;
            CORE_PROFILE_SCOPE("glfwInit");
            int success = glfwInit();
            CORE_LOG_INFO("GLFW initialized successfully");
            CORE_ASSERT(success, "GLFW initialization failed!");
            glfwSetErrorCallback(GLFWErrorCallback);

            // This is redundant since we check m_Window above, but keeping it for future safety
            // if i decide to handle window creation errors with a fallback on the caller side.
            if (!success)
                return util::unexpected(errors::WindowError::InitializationFailed);
        } else {
            CORE_ASSERT(false, "Window already exists!");
            return util::unexpected(errors::WindowError::WindowAlreadyExists);
        }

        {
            CORE_PROFILE_SCOPE("glfeCreateWindow");
#if defined(CORE_DEBUG)
            if (RendererAPI::Current() == RendererAPIType::Opengl) {
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
            }
#endif
            m_Window = UniqueGLFWwindow(glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title.c_str(), nullptr, nullptr), GLFWwindowDeleter{});
            CORE_ASSERT(m_Window, "Failed to create GLFW window!");
            CORE_LOG_INFO("GLFW Window '{0}' created successfully", title);

            m_RendererContext = RendererContext::Create(m_Window.get());
            CORE_ASSERT(m_RendererContext, "RendererContext creation failed!");
            m_RendererContext->Init();
            CORE_LOG_INFO("Initialized Context of type '{0}'", static_cast<int>(m_RendererContext->GetCurrentAPI()));

            SetVSync(true);
            CORE_LOG_INFO("Window Initialized successfully");
        }
        return {};
    }

    util::expected<void, errors::WindowError> Window::Shutdown() {
        CORE_PROFILE_FUNCTION();
        m_RendererContext.reset();
        m_Window.reset();
        CORE_LOG_INFO("Window Shutdown successfully");
        return {};
    }

    void Window::OnUpdate() {
        CORE_PROFILE_FUNCTION();
        glfwPollEvents();
        m_RendererContext->SwapBuffers();
    }

    void Window::SetVSync(bool enabled) {
        CORE_PROFILE_FUNCTION();

        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool Window::IsVSync() const {
        return m_Data.VSync;
    }

    Shared<Window> Window::Create(const std::string& title, unsigned int width, unsigned int height) {
        CORE_PROFILE_FUNCTION();
        return CreateShared<Window>(title, width, height);
    }
} // namespace ge