#include "OpenglContext.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace ge {
    OpenglContext::OpenglContext(GLFWwindow* window) : m_WindowHandle(window) {
        CORE_PROFILE_FUNCTION();
        CORE_ASSERT(window, "Window handle is null!");
    }

    void OpenglContext::Init() {
        CORE_PROFILE_FUNCTION();
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        CORE_ASSERT(status, "Failed to initialize Glad!");

        CORE_LOG_INFO("OpenGL Info:");
        CORE_LOG_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        CORE_LOG_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        CORE_LOG_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        // Initialize OpenGL loader here (e.g., glad, glew, etc.)
        // For example, if using glad:
        // int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        // CORE_ASSERT(status, "Failed to initialize OpenGL context!");

        CORE_LOG_INFO("OpenGL Context Initialized");
    }
    void OpenglContext::SwapBuffers() {
        CORE_PROFILE_FUNCTION();
        glfwSwapBuffers(m_WindowHandle);
    }
} // namespace ge
