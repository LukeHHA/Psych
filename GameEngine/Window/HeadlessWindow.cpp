#include "HeadlessWindow.h"
#include "Debug/Instrumentor.h"
#include "GLFW/glfw3.h"

namespace ge
{

static void GLFWErrorCallback(int error, const char* description) { CORE_LOG_ERROR("GLFW Error ({0}): {1}", error, description); }

HeadlessWindow::~HeadlessWindow()
{
  CORE_PROFILE_FUNCTION();
  {
    CORE_PROFILE_SCOPE("HeadlessWindow::Shutdown");
    Shutdown();
  }
}

Expected<void, errors::WindowError> HeadlessWindow::Init(const std::string& title, unsigned int width, unsigned int height, EventHandler& eventHandler)
{
  CORE_PROFILE_FUNCTION();

  if (width == 0 || height == 0) {
    return Unexpected(errors::WindowError::InvalidDimensions);
  }

  // This is just to ensure one window for now but will be reference counted in
  // the future
  if (!m_Window) {
    m_Data.Title  = title;
    m_Data.Width  = width;
    m_Data.Height = height;
    CORE_PROFILE_SCOPE("glfwInit");
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_NULL);
    int success = glfwInit();
    if (success == 0) {
      return Unexpected(errors::WindowError::InitializationFailed);
    }

    CORE_LOG_INFO("GLFW initialized successfully");
    glfwSetErrorCallback(GLFWErrorCallback);
  } else {
    return Unexpected(errors::WindowError::WindowAlreadyExists);
  }

  {
    CORE_PROFILE_SCOPE("glfeCreateWindow");
#if defined(CORE_DEBUG)
    if (RendererAPI::Current() == RendererAPIType::OPENGL) {
      glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    }
#endif
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = UniqueGLFWwindow(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr), GLFWwindowDeleter{});

    if (!m_Window) {
      return Unexpected(errors::WindowError::NativeWindowCreationFailed);
    }

    auto contextResult = RendererContext::Create(m_Window.get());
    if (!contextResult) {
      return Unexpected(errors::WindowError::ContextCreationFailed);
    }

    m_RendererContext = std::move(contextResult.value());
    auto contextInit  = m_RendererContext->Init();
    if (!contextInit) {
      return Unexpected(errors::WindowError::ContextInitializationFailed);
    }

    CORE_LOG_INFO("HeadlessWindow Initialized successfully");
  }
  return {};
}

Expected<void, errors::WindowError> HeadlessWindow::Shutdown()
{
  CORE_PROFILE_FUNCTION();
  CORE_LOG_INFO("HeadlessWindow Shutdown successfully");
  return {};
}

void HeadlessWindow::OnUpdate() { CORE_PROFILE_FUNCTION(); }

void HeadlessWindow::SetVSync(bool enabled) { m_Data.VSync = enabled; }

bool HeadlessWindow::IsVSync() const { return m_Data.VSync; }

void HeadlessWindow::HandleEvents(Unique<Event> event) {};
} // namespace ge
