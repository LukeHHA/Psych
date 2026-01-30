#include "EngineWindow.h"
#include "Debug/Instrumentor.h"
#include "Debug/Assert.h"

namespace ge {

    static void GLFWErrorCallback(int error, const char* description) {
        CORE_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    EngineWindow::EngineWindow(const std::string& title, unsigned int width, unsigned int height) {
        CORE_PROFILE_FUNCTION();
        {
            CORE_PROFILE_SCOPE("EngineWindow::Init");
            Init(title, width, height);
        }
    }

    EngineWindow::~EngineWindow() {
        CORE_PROFILE_FUNCTION();
        {
            CORE_PROFILE_SCOPE("EngineWindow::Shutdown");
            Shutdown();
        }
    }

    util::expected<void, errors::WindowError> EngineWindow::Init(const std::string& title, unsigned int width, unsigned int height) {
        CORE_PROFILE_FUNCTION();

        // This is just to ensure one window for now but will be reference counted in the future
        if (!m_Window) {
            m_Data.Title = title;
            m_Data.Width = width;
            m_Data.Height = height;
            CORE_PROFILE_SCOPE("glfwInit");
            int success = glfwInit();
            CORE_ASSERT(success, "GLFW initialization failed!");
            CORE_LOG_INFO("GLFW initialized successfully");
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
            if (RendererAPI::Current() == RendererAPIType::OPENGL) {
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
            }
#endif
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

            m_Window = UniqueGLFWwindow(glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title.c_str(), nullptr, nullptr), GLFWwindowDeleter{});
            CORE_ASSERT(m_Window, "Failed to create GLFW window!");
            CORE_LOG_INFO("GLFW Window '{0}' created successfully", title);

            m_RendererContext = RendererContext::Create(m_Window.get());
            CORE_ASSERT(m_RendererContext, "RendererContext creation failed!");
            m_RendererContext->Init();
            CORE_LOG_INFO("Initialized Context of type '{0}'", static_cast<int>(m_RendererContext->GetCurrentAPI()));

            SetVSync(true);
            CORE_LOG_INFO("EngineWindow Initialized successfully");
        }
        return {};
    }

    util::expected<void, errors::WindowError> EngineWindow::Shutdown() {
        CORE_PROFILE_FUNCTION();
        m_RendererContext.reset();
        m_Window.reset();
        CORE_LOG_INFO("EngineWindow Shutdown successfully");
        return {};
    }

    void EngineWindow::OnUpdate() {
        CORE_PROFILE_FUNCTION();
        glfwPollEvents();
        m_RendererContext->SwapBuffers();
    }

    void EngineWindow::SetVSync(bool enabled) {
        CORE_PROFILE_FUNCTION();

        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool EngineWindow::IsVSync() const {
        return m_Data.VSync;
    }

} // namespace ge