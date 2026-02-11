#include "GameEngine.h"
#include "Core/Base.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "Renderer/RendererAPI.h"

namespace ge
{

GameEngine* GameEngine::s_Application = nullptr;

GameEngine::GameEngine(const GameEngineSpecification& specification)
    : m_Specification(specification)
{
  CORE_PROFILE_FUNCTION();
  Init();
  CORE_LOG_INFO("Game Engine Initialized");
}

GameEngine::~GameEngine()
{
  CORE_PROFILE_FUNCTION();
  Shutdown();
  CORE_LOG_INFO("Game Engine Shutdown Complete");
}

util::expected<void, errors::EngineError> GameEngine::Init()
{
  CORE_PROFILE_FUNCTION();
  CORE_PROFILE_SCOPE("GameEngine::Init");

  CORE_ASSERT(!s_Application, "Application already exists")
  s_Application = this;

#ifdef GE_TESTS_ENABLED
  RendererAPI::SetAPI(RendererAPIType::TEST_HEADLESS);
#else
  RendererAPI::SetAPI(RendererAPIType::OPENGL);
#endif
  m_EventHandler_ = CreateShared<EventHandler>();
  // Will return a headless window when the tests are enabled
  m_Window = Window::Create("Game Engine", 1280, 720, m_EventHandler_);

  return {};
}

util::expected<void, errors::EngineError> GameEngine::Shutdown()
{
  CORE_PROFILE_FUNCTION();
  CORE_PROFILE_SCOPE("GameEngine::Shutdown");
  CORE_ASSERT(s_Application == this,
              "Static Application Pointer Is Corrupt On Teardown");
  s_Application = nullptr;
  return {};
}

void GameEngine::HandleEvents()
{
  Unique<Event> event = nullptr;
  while (m_EventHandler_->TryDequeueEvent(event)) {
    switch (event->GetEventType()) {
    case EventType::WindowClose:
      m_Running = false;
    }
    m_Window->HandleEvents(std::move(event));
  }
}

void GameEngine::Run()
{
  CORE_PROFILE_FUNCTION();
  m_Running = true;
  CORE_LOG_INFO("Entering Main Application Loop");

  // Main Application loop
  while (m_Running) {
    m_Window->PollEvents();

    HandleEvents();

    for (const auto& layer : m_LayerStack) layer->OnUpdate();

    for (const auto& layer : m_LayerStack) layer->OnRender();

    m_Window->OnUpdate();
  }
}

void GameEngine::Stop()
{
  CORE_PROFILE_FUNCTION();
  m_Running = false;
  CORE_LOG_INFO("Exiting Main Application Loop");
}

GameEngine& GameEngine::Get()
{
  CORE_PROFILE_FUNCTION();
  CORE_ASSERT(s_Application != nullptr,
              "Application is NULLPTR during call to GET()");
  return *s_Application;
}

void GameEngine::PushLayer(std::unique_ptr<Layer> layer)
{
  CORE_PROFILE_FUNCTION();
  m_LayerStack.PushLayer(std::move(layer));
}

void GameEngine::PushOverlay(std::unique_ptr<Layer> layer)
{
  CORE_PROFILE_FUNCTION();
  m_LayerStack.PushOverlay(std::move(layer));
}
} // namespace ge
