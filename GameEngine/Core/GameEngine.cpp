#include "GameEngine.h"
#include "Core/Core.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "EmbeddedResources.h"
#include "FileSystem/EngineFilesystem.h"
#include "Fonts/FontManager.h"
#include "Imgui/ImguiLayer.h"
#include "Renderer/Renderer.h"
#include "Util/Time.h"

#include <filesystem>
#include <utility>

namespace ge
{

GameEngine* GameEngine::s_Application = nullptr;

GameEngine::GameEngine()
{
  CORE_LOG_INFO("Game engine startup");
  CORE_ASSERT(!s_Application, "Application already exists")
  s_Application = this;
}

Expected<void, errors::EngineError> GameEngine::Init()
{
  CORE_PROFILE_FUNCTION();
  CORE_PROFILE_SCOPE("GameEngine::Init");

  if (m_Initialized || m_LayerStack || m_EventHandler_ || m_Window) {
    return Unexpected(errors::EngineError::InvalidGameEngineState);
  }

  auto configResult = m_Config.Init();
  if (!configResult) {
    return Unexpected(errors::EngineError::GameEngineInitializationFailed);
  }
  auto spec = m_Config.GetGameEngineSpec();

  std::error_code resourcePathError;
  const auto resourcePath = std::filesystem::current_path(resourcePathError);
  if (resourcePathError) {
    return Unexpected(errors::EngineError::GameEngineInitializationFailed);
  }
  m_PathResolver_.SetEngineRoot(resourcePath);

  const auto projectResult = m_ProjectManager_.Init(m_PathResolver_);
  if (!projectResult) {
    return Unexpected(errors::EngineError::GameEngineInitializationFailed);
  }
  util::EngineFilesystem::Init(m_PathResolver_);

  m_LayerStack    = CreateUnique<LayerStack>();
  m_EventHandler_ = CreateShared<EventHandler>();

  Renderer::SetRendererAPI(spec.RenderingAPI);
  auto window = Window::Create(spec.Name, 1280, 720, m_EventHandler_);
  if (!window) {
    return Unexpected(errors::EngineError::WindowCreationFailed);
  }
  m_Window      = window.value();

  auto renderer = Renderer::Init(spec);
  if (!renderer) {
    return Unexpected(errors::EngineError::RendererInitializationFailed);
  }

  if (spec.EnableEditorUI) {
    m_Framebuffer_ = Framebuffer::Create(m_Window->GetWidth(), m_Window->GetHeight());
    if (!m_Framebuffer_) {
      return Unexpected(errors::EngineError::FramebufferCreationFailed);
    }
    m_RenderTarget_ = CreateUnique<FramebufferRenderTarget>(m_Framebuffer_);
    PushLayer(CreateUnique<ImGuiLayer>());
  } else {
    m_RenderTarget_ = CreateUnique<WindowRenderTarget>(*m_Window);
  }

  CORE_ASSERT(m_EventHandler_, "EventHandler creation failed")
  CORE_ASSERT(m_Window, "Window Creation failed returning nullptr")
  m_Initialized = true;
  CORE_LOG_INFO("GameEngine Init");
  return {};
}

GameEngine::~GameEngine()
{
  const auto shutdownResult = Shutdown();
  if (!shutdownResult) {
    CORE_LOG_ERROR("GameEngine shutdown failed");
  }
}

Expected<void, errors::EngineError> GameEngine::Shutdown()
{
  CORE_PROFILE_FUNCTION();
  CORE_PROFILE_SCOPE("GameEngine::Shutdown");

  if (!m_Initialized && !m_LayerStack && !m_Window && !m_EventHandler_) {
    return {};
  }

  m_Running = false;
  m_LayerStack.reset();
  m_RenderTarget_.reset();
  m_Framebuffer_.reset();
  Renderer::Shutdown();
  m_Window.reset();
  m_EventHandler_.reset();

  m_Initialized            = false;
  s_Application            = nullptr;
  const auto projectResult = m_ProjectManager_.Shutdown(m_PathResolver_);
  util::EngineFilesystem::Shutdown();
  m_PathResolver_.ClearEngineRoot();
  if (!projectResult) {
    return Unexpected(errors::EngineError::GameEngineInitializationFailed);
  }

  const auto configResult = m_Config.Shutdown();
  if (!configResult) {
    return Unexpected(errors::EngineError::GameEngineInitializationFailed);
  }

  CORE_LOG_INFO("GameEngine Shutdown");
  return {};
}

void GameEngine::HandleEvents()
{
  CORE_PROFILE_FUNCTION();
  Unique<Event> event = nullptr;
  while (m_EventHandler_->TryDequeueEvent(event)) {
    switch (event->GetEventType()) {
    case EventType::WindowClose:
      m_Running = false;
    case EventType::WindowShouldClose:
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
    Time::Update();

    m_Window->PollEvents();

    HandleEvents();

    for (const auto& layer : Layers()) {
      layer->OnUpdate();
    }

    CORE_ASSERT(m_RenderTarget_, "Engine render target is not initialized")
    Renderer::BeginScene(*m_RenderTarget_);

    for (const auto& layer : Layers()) {
      layer->OnRender();
    }

    Renderer::EndScene();

    if (GetEngineSpecification().EnableEditorUI) {
      Renderer::Clear();
      ImGuiLayer::Begin();

      for (const auto& layer : Layers()) {
        layer->OnImGuiRender();
      }

      ImGuiLayer::End();
    }

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
  CORE_ASSERT(s_Application != nullptr, "Application is NULLPTR during call to GET()");
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

const GameEngineConfig& GameEngine::GetConfig() const
{
  CORE_ASSERT(s_Application, "GameEngine does not exist yet");
  return m_Config;
}

const GameEngineSpecification& GameEngine::GetEngineSpecification() const { return GetConfig().GetGameEngineSpec(); }

ProjectManager& GameEngine::GetProjectManager() { return m_ProjectManager_; }

const ProjectManager& GameEngine::GetProjectManager() const { return m_ProjectManager_; }

const FontManager& GameEngine::GetFontLibrary() const { return m_FontLibrary_; }

const Shared<EventHandler>& GameEngine::GetEventHandler() const { return m_EventHandler_; }
} // namespace ge
