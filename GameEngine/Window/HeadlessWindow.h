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
  using EventCallbackFn                            = std::function<void(Event&)>;

  HeadlessWindow()                                 = default;
  HeadlessWindow(const HeadlessWindow&)            = delete;
  HeadlessWindow(HeadlessWindow&&)                 = delete;
  HeadlessWindow& operator=(const HeadlessWindow&) = delete;
  HeadlessWindow& operator=(HeadlessWindow&&)      = delete;
  ~HeadlessWindow() override;

  Expected<void, errors::WindowError> Init(const std::string& title, unsigned int width, unsigned int height, EventHandler& eventHandler) override;
  Expected<void, errors::WindowError> Shutdown() override;
  void PollEvents() override {}
  [[nodiscard]] unsigned int GetWidth() const override { return m_Data.Width; }
  [[nodiscard]] unsigned int GetHeight() const override { return m_Data.Height; }
  [[nodiscard]] GLFWwindow* GetNativeWindow() const override { return m_Window.get(); }
  void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
  void OnUpdate() override;
  void SetVSync(bool enabled) override;
  void HandleEvents(Unique<Event> event) override;
  [[nodiscard]] bool IsVSync() const override;

private:
  Unique<RendererContext> m_RendererContext;
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
