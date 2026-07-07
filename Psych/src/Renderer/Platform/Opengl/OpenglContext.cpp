#include "OpenglContext.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "glad/glad.h"

namespace psych
{
OpenglContext::OpenglContext(GLFWwindow* window) : m_WindowHandle(window) { CORE_PROFILE_FUNCTION(); }

Expected<void, errors::RendererError> OpenglContext::Init()
{
  CORE_PROFILE_FUNCTION();
  if (m_WindowHandle == nullptr) {
    return Unexpected(errors::RendererError::ContextCreationFailed);
  }

  glfwMakeContextCurrent(m_WindowHandle);
  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (!status) {
    return Unexpected(errors::RendererError::ContextInitializationFailed);
  }

  CORE_LOG_INFO("OpenGL Info:");
  CORE_LOG_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
  CORE_LOG_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
  CORE_LOG_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
  CORE_LOG_INFO("OpenGL Context Initialized");
  GLint major = 0, minor = 0;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  CORE_LOG_INFO("Opengl major version: {0}", major);
  CORE_LOG_INFO("Opengl minor version {0}", minor);
  return {};
}
void OpenglContext::SwapBuffers()
{
  CORE_PROFILE_FUNCTION();
  glfwSwapBuffers(m_WindowHandle);
}

} // namespace psych
