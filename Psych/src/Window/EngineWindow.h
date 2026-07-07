#pragma once

#include "Core/Core.h"
#include "Core/Window.h"
#include "Errors/Errors.h"
#include "Events/Event.h"
#include "Renderer/RendererContext.h"
#include "expected.h"

#include <functional>
#include <string>

namespace psych
{
class EngineWindow : public Window
{
public:
  using EventCallbackFn                        = std::function<void(Event&)>;
  using QueueEventFn                           = EventCallbackFn;

  EngineWindow()                               = default;
  EngineWindow(const EngineWindow&)            = delete;
  EngineWindow(EngineWindow&&)                 = delete;
  EngineWindow& operator=(const EngineWindow&) = delete;
  EngineWindow& operator=(EngineWindow&&)      = delete;
  ~EngineWindow() override;

  Expected<void, errors::WindowError> Init(const std::string& title, unsigned int width, unsigned int height, EventHandler& eventHandler) override;
  Expected<void, errors::WindowError> Shutdown() override;
  void PollEvents() override;
  [[nodiscard]] unsigned int GetWidth() const override { return m_Data.Width; }
  [[nodiscard]] unsigned int GetHeight() const override { return m_Data.Height; }
  [[nodiscard]] GLFWwindow* GetNativeWindow() const override { return m_Window.get(); }
  void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
  void OnUpdate() override;
  void HandleEvents(Unique<Event> event) override;
  void SetVSync(bool enabled) override;
  [[nodiscard]] bool IsVSync() const override;

private:
  Unique<RendererContext> m_RendererContext;
  UniqueGLFWwindow m_Window;

  struct WindowData {
    std::string Title;
    unsigned int Width, Height;
    bool VSync;
    EventHandler* EventsHandler = nullptr;
    EventCallbackFn EventCallback;
  };

  WindowData m_Data;
};

} // namespace psych
