#include "HeadlessWindow.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"

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

util::expected<void, errors::WindowError>
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
    int success = glfwInit();
    CORE_ASSERT(success, "GLFW initialization failed!");
    CORE_LOG_INFO("GLFW initialized successfully");
    glfwSetErrorCallback(GLFWErrorCallback);

    // This is redundant since we check m_Window above, but keeping it for
    // future safety if i decide to handle window creation errors with a
    // fallback on the caller side.
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

    SetVSync(true);
    CORE_LOG_INFO("HeadlessWindow Initialized successfully");
  }
  return {};
}

util::expected<void, errors::WindowError> HeadlessWindow::Shutdown()
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
