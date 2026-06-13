#include "HeadlessWindow.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "GLFW/glfw3.h"

namespace ge
{

static void GLFWErrorCallback(int error, const char* description)
{
  CORE_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}

HeadlessWindow::HeadlessWindow(const std::string& title, unsigned int width,
                               unsigned int height,
                               Shared<EventHandler> eventHandler)
{
  CORE_PROFILE_FUNCTION();
  {
    CORE_PROFILE_SCOPE("HeadlessWindow::Init");
    Init(title, width, height, eventHandler);
  }
}

HeadlessWindow::~HeadlessWindow()
{
  CORE_PROFILE_FUNCTION();
  {
    CORE_PROFILE_SCOPE("HeadlessWindow::Shutdown");
    Shutdown();
  }
}

Expected<void, errors::WindowError>
HeadlessWindow::Init(const std::string& title, unsigned int width,
                     unsigned int height, Shared<EventHandler> eventHandler)
{
  CORE_PROFILE_FUNCTION();

  // This is just to ensure one window for now but will be reference counted in
  // the future
  if (!m_Window) {
    m_Data.Title  = title;
    m_Data.Width  = width;
    m_Data.Height = height;
    CORE_PROFILE_SCOPE("glfwInit");
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_NULL);
    int success = glfwInit();
    CORE_ASSERT(success, "GLFW initialization failed!");
    CORE_LOG_INFO("GLFW initialized successfully");
    glfwSetErrorCallback(GLFWErrorCallback);

    // This is redundant since we check m_Window above, but keeping it for
    // future safety if i decide to handle window creation errors with a
    // fallback on the caller side.
    if (!success)
      return Unexpected(errors::WindowError::InitializationFailed);
  } else {
    CORE_ASSERT(false, "Window already exists!");
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

    m_Window = UniqueGLFWwindow(
        glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr),
        GLFWwindowDeleter{});

    CORE_ASSERT(m_Window, "Failed to create headless GLFW window");

    m_RendererContext = RendererContext::Create(m_Window.get());
    CORE_ASSERT(m_RendererContext, "RendererContext creation failed");

    m_RendererContext->Init();
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
