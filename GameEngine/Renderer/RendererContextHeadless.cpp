#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "RendererContext.h"
#include "glad/glad.h"
namespace ge
{
RendererContextHeadless::RendererContextHeadless(GLFWwindow* window)
    : m_WindowHandle(window)
{
  CORE_PROFILE_FUNCTION();
  CORE_ASSERT(window, "Window handle is null!");
}

void RendererContextHeadless::Init()
{
  CORE_PROFILE_FUNCTION();
  glfwMakeContextCurrent(m_WindowHandle);
  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  CORE_ASSERT(status, "Failed to initialize Glad!");

  CORE_LOG_INFO("OpenGL Info:");
  CORE_LOG_INFO("  Vendor: {0}",
                reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
  CORE_LOG_INFO("  Renderer: {0}",
                reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
  CORE_LOG_INFO("  Version: {0}",
                reinterpret_cast<const char*>(glGetString(GL_VERSION)));
  CORE_LOG_INFO("OpenGL Context Initialized");
  GLint major = 0, minor = 0;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  CORE_LOG_INFO("Opengl major version: {0}", major);
  CORE_LOG_INFO("Opengl minor version {0}", minor);
}
void RendererContextHeadless::SwapBuffers()
{
  CORE_PROFILE_FUNCTION();
  glfwSwapBuffers(m_WindowHandle);
}

} // namespace ge
