#pragma once

#include "Core/Core.h"
#include "Core/Window.h"
#include "Core/glad_glfw_incl.h"
#include "Errors/Errors.h"
#include "Events/Event.h"
#include "Renderer/RendererContext.h"
#include "ge_expected"

#include <functional>
#include <string>

namespace ge
{
class HeadlessWindow : public Window
{
public:
  using EventCallbackFn = std::function<void(Event&)>;

  HeadlessWindow(const std::string& title, unsigned int width,
                 unsigned int height, Shared<EventHandler> eventHandler);
  virtual ~HeadlessWindow();

  Expected<void, errors::WindowError>
  Init(const std::string& title, unsigned int width, unsigned int height,
       Shared<EventHandler> eventHandler) override;
  Expected<void, errors::WindowError> Shutdown() override;
  virtual void PollEvents() override {}
  virtual unsigned int GetWidth() const override { return m_Data.Width; }
  virtual unsigned int GetHeight() const override { return m_Data.Height; }
  virtual GLFWwindow* GetNativeWindow() const override
  {
    return m_Window.get();
  }
  virtual void SetEventCallback(const EventCallbackFn& callback) override
  {
    m_Data.EventCallback = callback;
  }
  void OnUpdate() override;
  void SetVSync(bool enabled) override;
  virtual void HandleEvents(Unique<Event> event) override;
  bool IsVSync() const override;

private:
  Shared<RendererContext> m_RendererContext;
  UniqueGLFWwindow m_Window;

  struct WindowData {
    std::string Title;
    unsigned int Width, Height;
    bool VSync;

    EventCallbackFn EventCallback;
  };

  WindowData m_Data;
};

} // namespace ge
