#include "EngineWindow.h"
#include "Core/Base.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "Events/Event.h"
#include "Events/EventHandler.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvents.h"
#include <GLFW/glfw3.h>

namespace ge
{
static void GLFWErrorCallback(int error, const char* description)
{
  CORE_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}

EngineWindow::EngineWindow(const std::string& title, unsigned int width,
                           unsigned int height,
                           Shared<EventHandler> eventHandler)

{
  CORE_PROFILE_FUNCTION();
  {
    CORE_PROFILE_SCOPE("EngineWindow::Init");
    Init(title, width, height, eventHandler);
  }
  CORE_LOG_INFO("Window Init");
}

EngineWindow::~EngineWindow()
{
  CORE_PROFILE_FUNCTION();
  {
    CORE_PROFILE_SCOPE("EngineWindow::Shutdown");
    Shutdown();
  }
  CORE_LOG_INFO("Window Shutdown");
}

util::expected<void, errors::WindowError>
EngineWindow::Init(const std::string& title, unsigned int width,
                   unsigned int height, Shared<EventHandler> eventHandler)
{
  CORE_PROFILE_FUNCTION();

  // This is just to ensure one window for now but will be reference counted in
  // the future
  if (!m_Window) {
    m_Data.Title         = title;
    m_Data.Width         = width;
    m_Data.Height        = height;
    m_Data.EventsHandler = eventHandler;

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

    m_Window = UniqueGLFWwindow(
        glfwCreateWindow(static_cast<int>(width), static_cast<int>(height),
                         title.c_str(), nullptr, nullptr),
        GLFWwindowDeleter{});
    CORE_ASSERT(m_Window, "Failed to create GLFW window!");
    CORE_LOG_INFO("GLFW Window '{0}' created successfully", title);

    m_RendererContext = RendererContext::Create(m_Window.get());
    CORE_ASSERT(m_RendererContext, "RendererContext creation failed!");
    m_RendererContext->Init();
    CORE_LOG_INFO("Initialized Context of type '{0}'",
                  static_cast<int>(m_RendererContext->GetCurrentAPI()));

    glfwSetWindowUserPointer(m_Window.get(), &m_Data);
    SetVSync(true);
    CORE_LOG_INFO("EngineWindow Initialized successfully");

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(
        m_Window.get(), [](GLFWwindow* window, int width, int height) {
          WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

          auto event       = CreateUnique<WindowResizeEvent>(width, height);
          data.EventsHandler->QueueEvent(std::move(event));
        });

    glfwSetFramebufferSizeCallback(
        m_Window.get(), [](GLFWwindow* window, int width, int height) {
          WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
          auto event = CreateUnique<FramebufferResizeEvent>(width, height);
          data.EventsHandler->QueueEvent(std::move(event));
        });

    glfwSetWindowCloseCallback(m_Window.get(), [](GLFWwindow* window) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      auto event       = CreateUnique<WindowCloseEvent>();
      data.EventsHandler->QueueEvent(std::move(event));
    });

    glfwSetKeyCallback(m_Window.get(), [](GLFWwindow* window, int key,
                                          int scancode, int action, int mods) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      switch (action) {
      case GLFW_PRESS: {
        auto event = CreateUnique<KeyPressedEvent>(key, 0);
        data.EventsHandler->QueueEvent(std::move(event));
        break;
      }
      case GLFW_RELEASE: {
        auto event = CreateUnique<KeyReleasedEvent>(key);
        data.EventsHandler->QueueEvent(std::move(event));
        break;
      }
      case GLFW_REPEAT: {
        auto event = CreateUnique<KeyPressedEvent>(key, true);
        data.EventsHandler->QueueEvent(std::move(event));
        break;
      }
      }
    });

    glfwSetCharCallback(
        m_Window.get(), [](GLFWwindow* window, unsigned int keycode) {
          WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
          auto event       = CreateUnique<KeyTypedEvent>(keycode);
          data.EventsHandler->QueueEvent(std::move(event));
        });

    glfwSetMouseButtonCallback(
        m_Window.get(),
        [](GLFWwindow* window, int button, int action, int mods) {
          WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

          switch (action) {
          case GLFW_PRESS: {
            auto event = CreateUnique<MouseButtonPressedEvent>(button);
            data.EventsHandler->QueueEvent(std::move(event));
            break;
          }
          case GLFW_RELEASE: {
            auto event = CreateUnique<MouseButtonReleasedEvent>(button);
            data.EventsHandler->QueueEvent(std::move(event));
            break;
          }
          }
        });

    glfwSetScrollCallback(
        m_Window.get(), [](GLFWwindow* window, double xOffset, double yOffset) {
          WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

          auto event =
              CreateUnique<MouseScrolledEvent>((float)xOffset, (float)yOffset);
          data.EventsHandler->QueueEvent(std::move(event));
        });

    glfwSetCursorPosCallback(
        m_Window.get(), [](GLFWwindow* window, double xPos, double yPos) {
          WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

          auto event = CreateUnique<MouseMovedEvent>((float)xPos, (float)yPos);
          data.EventsHandler->QueueEvent(std::move(event));
        });
  }
  return {};
}

util::expected<void, errors::WindowError> EngineWindow::Shutdown()
{
  CORE_PROFILE_FUNCTION();
  m_RendererContext.reset();
  m_Window.reset();
  CORE_LOG_INFO("EngineWindow Shutdown successfully");
  return {};
}

void EngineWindow::PollEvents() { glfwPollEvents(); }

void EngineWindow::OnUpdate()
{
  CORE_PROFILE_FUNCTION();
  m_RendererContext->SwapBuffers();
}

void EngineWindow::HandleEvents(Unique<Event> event)
{
  switch (event->GetEventType()) {
  case EventType::WindowResize:
    auto& e       = static_cast<WindowResizeEvent&>(*event);
    m_Data.Height = e.GetHeight();
    m_Data.Width  = e.GetWidth();
    std::cout << "window resize\n";
  }
}
void EngineWindow::SetVSync(bool enabled)
{
  CORE_PROFILE_FUNCTION();

  if (enabled)
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);

  m_Data.VSync = enabled;
}

bool EngineWindow::IsVSync() const { return m_Data.VSync; }

} // namespace ge
