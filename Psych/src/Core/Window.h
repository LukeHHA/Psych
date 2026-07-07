#pragma once

#include "Core/Core.h"
#include "Core/glad_glfw_incl.h"
#include "Errors/Errors.h"
#include "Events/Event.h"
#include "Events/EventHandler.h"
#include "Renderer/RendererContext.h"
#include "expected.h"

#include <functional>
#include <string>

namespace psych
{
struct GLFWwindowDeleter {
  void operator()(GLFWwindow* window) const noexcept
  {
    // Custom deleter logic for GLFWwindow
    if (window != nullptr) {
      // Assume glfwDestroyWindow is the function to destroy a
      // GLFWwindow
      glfwDestroyWindow(window);
    }
  }
};

using UniqueGLFWwindow = Unique<GLFWwindow, GLFWwindowDeleter>;

class Window
{
public:
  using EventCallbackFn       = std::function<void(Event&)>;
  using QueueEventFn          = EventCallbackFn;

  Window()                    = default;
  virtual ~Window()           = default;
  Window(const Window& other) = delete;
  Window(Window&& other)      = delete; // Moving may be allowed in the future if it
                                        // the window ever needs to change owners
  Window& operator=(const Window& other)                                                                                                          = delete;
  Window& operator=(Window&& other)                                                                                                               = delete;

  virtual Expected<void, errors::WindowError> Init(const std::string& title, unsigned int width, unsigned int height, EventHandler& eventHandler) = 0;
  virtual Expected<void, errors::WindowError> Shutdown()                                                                                          = 0;
  virtual void OnUpdate()                                                                                                                         = 0;
  virtual void PollEvents()                                                                                                                       = 0;
  [[nodiscard]] virtual unsigned int GetWidth() const                                                                                             = 0;
  [[nodiscard]] virtual unsigned int GetHeight() const                                                                                            = 0;
  [[nodiscard]] virtual GLFWwindow* GetNativeWindow() const                                                                                       = 0;
  virtual void SetEventCallback(const EventCallbackFn& callback)                                                                                  = 0;
  virtual void SetVSync(bool enabled)                                                                                                             = 0;
  [[nodiscard]] virtual bool IsVSync() const                                                                                                      = 0;
  virtual void HandleEvents(Unique<Event> event)                                                                                                  = 0;
  static Expected<Unique<Window>, errors::WindowError> Create(const std::string& title, unsigned int width, unsigned int height, EventHandler& eventHandler);

private:
  Unique<RendererContext> m_RendererContext;
  UniqueGLFWwindow m_Window;

  struct WindowData {
    std::string Title;
    unsigned int Width, Height;
    bool VSync;
    EventHandler EventsHandler;
    EventCallbackFn EventCallback;
  };

  WindowData m_Data;
};

} // namespace psych
