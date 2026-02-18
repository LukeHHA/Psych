#pragma once

#include "Core/Base.h"
#include "Core/Window.h"
#include "Errors/Errors.h"
#include "Events/EventHandler.h"
#include "Layers/LayerStack.h"
#include "Renderer/RendererAPI.h"
#include "ge_expected"

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

  util::expected<void, errors::EngineError> Init();
  util::expected<void, errors::EngineError> Shutdown();
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
  GameEngineSpecification m_Specification;
  bool m_Running = false;
  static GameEngine* s_Application;
  LayerStack m_LayerStack;
  Shared<Window> m_Window;
  Shared<EventHandler> m_EventHandler_;
};
Unique<GameEngine> CreateGameEngine();
} // namespace ge
