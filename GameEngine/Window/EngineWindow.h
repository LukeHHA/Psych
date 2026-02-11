#pragma once

#include "Core/Base.h"
#include "Core/Window.h"
#include "Errors/Errors.h"
#include "Events/Event.h"
#include "GLFW/glfw3.h"
#include "Renderer/RendererContext.h"
#include "ge_expected"

#include <functional>
#include <string>

namespace ge
{
class EventHandler;
class EngineWindow : public Window
{
public:
  using EventCallbackFn = std::function<void(Event&)>;
  using QueueEventFn    = EventCallbackFn;

  EngineWindow(const std::string& title, unsigned int width,
               unsigned int height, Shared<EventHandler> eventHandler);
  virtual ~EngineWindow();

  virtual util::expected<void, errors::WindowError>
  Init(const std::string& title, unsigned int width, unsigned int height,
       Shared<EventHandler> eventHandler) override;
  virtual util::expected<void, errors::WindowError> Shutdown() override;
  virtual void PollEvents() override;
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
  virtual void OnUpdate() override;
  virtual void HandleEvents(Unique<Event> event) override;
  virtual void SetVSync(bool enabled) override;
  virtual bool IsVSync() const override;

private:
  Shared<RendererContext> m_RendererContext;
  UniqueGLFWwindow m_Window;

  struct WindowData {
    std::string Title;
    unsigned int Width, Height;
    bool VSync;
    Shared<EventHandler> EventHandler;
    EventCallbackFn EventCallback;
  };

  WindowData m_Data;
};

} // namespace ge
