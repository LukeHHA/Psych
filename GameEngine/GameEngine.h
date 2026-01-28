#pragma once

#include "Layers/LayerStack.h"

namespace Core {
struct GameEngineSpecification {
  std::string Name = "Application";
};

class GameEngine {
public:
  GameEngine(
      const GameEngineSpecification &specification = GameEngineSpecification());
  ~GameEngine();

  void Run();
  void Stop();

  void PushLayer(std::unique_ptr<Layer> layer);
  void PushOverlay(std::unique_ptr<Layer> layer);

  static GameEngine &Get();

private:
  GameEngineSpecification m_Specification;
  bool m_Running = false;
  static GameEngine *s_Application;
  LayerStack m_LayerStack;
};

std::unique_ptr<GameEngine> CreateGameEngine();
} // namespace Core
