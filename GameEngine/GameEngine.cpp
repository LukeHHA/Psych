#include "GameEngine.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"

namespace Core {

GameEngine *GameEngine::s_Application = nullptr;

GameEngine::GameEngine(const GameEngineSpecification &specification)
    : m_Specification(specification) {
  CORE_PROFILE_FUNCTION();
  CORE_ASSERT(!s_Application, "Application already exists")
  s_Application = this;
}

GameEngine::~GameEngine() {
  CORE_PROFILE_FUNCTION();
  CORE_ASSERT(s_Application == this,
              "Static Application Pointer Is Corrupt On Teardown");
  s_Application = nullptr;
}

void GameEngine::Run() {
  CORE_PROFILE_FUNCTION();
  m_Running = true;

  // Main Application loop
  while (m_Running) {
    for (const std::unique_ptr<Layer> &layer : m_LayerStack)
      layer->OnUpdate();

    for (const std::unique_ptr<Layer> &layer : m_LayerStack)
      layer->OnRender();
  }
}

void GameEngine::Stop() {
  CORE_PROFILE_FUNCTION();
  m_Running = false;
}

GameEngine &GameEngine::Get() {
  CORE_PROFILE_FUNCTION();
  CORE_ASSERT(s_Application != nullptr,
              "Application is NULLPTR during call to GET()");
  return *s_Application;
}

void GameEngine::PushLayer(std::unique_ptr<Layer> layer) {
  CORE_PROFILE_FUNCTION();
  m_LayerStack.PushLayer(std::move(layer));
}

void GameEngine::PushOverlay(std::unique_ptr<Layer> layer) {
  CORE_PROFILE_FUNCTION();
  m_LayerStack.PushOverlay(std::move(layer));
}
} // namespace Core
