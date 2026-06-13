#include "GameEngine.h"
#include "Core/Base.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "FileSystem/FileSystem.h"
#include "Imgui/ImguiLayer.h"
#include "Renderer/Renderer.h"

namespace ge
{

GameEngine* GameEngine::s_Application = nullptr;

GameEngine::GameEngine(const GameEngineSpecification& specification)
    : m_Specification(specification)
{
  CORE_LOG_INFO("Game engine startup");
  CORE_PROFILE_FUNCTION();
  CORE_PROFILE_SCOPE("GameEngine::Init");

  CORE_ASSERT(!s_Application, "Application already exists")
  s_Application   = this;

  m_LayerStack    = CreateUnique<LayerStack>();
  m_EventHandler_ = CreateShared<EventHandler>();
  Renderer::Init(m_Specification);
  m_Window = Window::Create("Game Engine", 1280, 720, m_EventHandler_);

  // WARN: Do not remove
  PushLayer(CreateUnique<ImGuiLayer>());
  CORE_ASSERT(m_EventHandler_, "EventHandler creation failed")
  CORE_ASSERT(m_EventHandler_, "EventHandler creation failed")
  CORE_ASSERT(m_Window, "Window Creation failed returning nullptr")
  CORE_LOG_INFO("GameEngine Init");
}

GameEngine::~GameEngine()
{
  CORE_PROFILE_FUNCTION();
  CORE_PROFILE_SCOPE("GameEngine::Shutdown");

  m_LayerStack.reset();
  m_EventHandler_.reset();
  m_Window.reset();

  s_Application = nullptr;
  CORE_LOG_INFO("GameEngine Shutdown");
}

void GameEngine::HandleEvents()
{
  CORE_PROFILE_FUNCTION();
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

  Renderer::SetClearColour({0.1f, 0.5f, 0.9f});

  // Main Application loop
  while (m_Running) {
    m_Window->PollEvents();

    HandleEvents();

    for (const auto& layer : Layers()) layer->OnUpdate();

    for (const auto& layer : Layers()) layer->OnRender();

    // NOTE: Imgui context is setup here using static methods
    // while the imgui frames are injected by the client. This
    // may change to a more complete imgui layer depending on
    // how much the core editor should take on UI wise.
    ImGuiLayer::Begin();
    for (const auto& layer : Layers()) layer->OnImGuiRender();
    ImGuiLayer::End();

    m_Window->OnUpdate();

    Renderer::Clear();
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
  m_LayerStack->PushLayer(std::move(layer));
}

void GameEngine::PushOverlay(std::unique_ptr<Layer> layer)
{
  CORE_PROFILE_FUNCTION();
  m_LayerStack->PushOverlay(std::move(layer));
}
} // namespace ge
