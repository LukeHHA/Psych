#pragma once

#include "Core/Base.h"
#include "Core/Window.h"
#include "Debug/Assert.h"
#include "Events/EventHandler.h"
#include "Imgui/ImguiLayer.h"
#include "Layers/LayerStack.h"
#include "Renderer/RendererAPI.h"

namespace ge
{
struct GameEngineSpecification {
  std::string Name             = "Application";
  RendererAPIType RenderingAPI = RendererAPIType::OPENGL;
};

class GameEngine
{
public:
  GameEngine(
      const GameEngineSpecification& specification = GameEngineSpecification());
  virtual ~GameEngine();
  CORE_NO_COPY_NO_MOVE(GameEngine);

  void Run();
  void Stop();
  void HandleEvents();
  void PushLayer(std::unique_ptr<Layer> layer);
  void PushOverlay(std::unique_ptr<Layer> layer);
  Window& GetWindow()
  {
    CORE_ASSERT(m_Window != nullptr,
                "Call to: GetWindow() failed. m_Window is nullptr!");
    return *m_Window;
  }
  static GameEngine& Get();

private:
  LayerStack& Layers()
  {
    CORE_ASSERT(m_LayerStack, "LayerStack is nullptr");
    return *m_LayerStack;
  }
  const LayerStack& Layers() const
  {
    CORE_ASSERT(m_LayerStack, "Layerstack is nullptr");
    return *m_LayerStack;
  }

private:
  GameEngineSpecification m_Specification;
  bool m_Running = false;
  static GameEngine* s_Application;
  Unique<LayerStack> m_LayerStack;
  ImGuiLayer* m_ImGuiLayer_;
  Shared<Window> m_Window;
  Shared<EventHandler> m_EventHandler_;
};
Unique<GameEngine> CreateGameEngine();
} // namespace ge
