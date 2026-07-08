 
/**************************************************************************/
/*  PsychEngine.cpp                                                       */                                                            
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PSYCH ENGINE                               */
/**************************************************************************/
/* Copyright (c)  Luke Howe                                               */                                                  
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "PsychEngine.h"
#include "Core/Core.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "FileSystem/EngineFilesystem.h"
#include "Fonts/FontManager.h"
#include "Imgui/ImguiLayer.h"
#include "Renderer/Renderer.h"
#include "Util/Time.h"

#include <filesystem>
#include <utility>

namespace psych
{

PsychEngine* PsychEngine::s_Application = nullptr;

PsychEngine::PsychEngine()
{
  CORE_LOG_INFO("Game engine startup");
  CORE_ASSERT(!s_Application, "Application already exists")
  s_Application = this;
}

Expected<void, errors::EngineError> PsychEngine::Init()
{
  CORE_PROFILE_FUNCTION();
  CORE_PROFILE_SCOPE("PsychEngine::Init");

  if (m_Initialized || m_LayerStack || m_Window) {
    return Unexpected(errors::EngineError::InvalidPsychEngineState);
  }

  auto configResult = m_Config.Init();
  if (!configResult) {
    return Unexpected(errors::EngineError::PsychEngineInitializationFailed);
  }
  auto spec = m_Config.GetPsychEngineSpec();

  std::error_code resourcePathError;
  const auto resourcePath = std::filesystem::current_path(resourcePathError);
  if (resourcePathError) {
    return Unexpected(errors::EngineError::PsychEngineInitializationFailed);
  }

  const auto projectResult = m_ProjectManager_.Init(m_PathResolver_);
  if (!projectResult) {
    return Unexpected(errors::EngineError::PsychEngineInitializationFailed);
  }
  util::EngineFilesystem::Init(m_PathResolver_);

  m_LayerStack = CreateUnique<LayerStack>();

  Renderer::SetRendererAPI(spec.RenderingAPI);
  auto window = Window::Create(spec.Name, 1280, 720, m_EventHandler_);
  if (!window) {
    return Unexpected(errors::EngineError::WindowCreationFailed);
  }
  m_Window      = std::move(window.value());

  auto renderer = Renderer::Init(spec);
  if (!renderer) {
    return Unexpected(errors::EngineError::RendererInitializationFailed);
  }

  if (spec.EnableEditorUI) {
    m_Framebuffer_ = Framebuffer::Create(m_Window->GetWidth(), m_Window->GetHeight());
    if (!m_Framebuffer_) {
      return Unexpected(errors::EngineError::FramebufferCreationFailed);
    }
    m_RenderTarget_ = CreateUnique<FramebufferRenderTarget>(*m_Framebuffer_);
    PushLayer(CreateUnique<ImGuiLayer>());
  } else {
    m_RenderTarget_ = CreateUnique<WindowRenderTarget>(*m_Window);
  }

  CORE_ASSERT(m_Window, "Window Creation failed returning nullptr")
  m_Initialized = true;
  CORE_LOG_INFO("PsychEngine Init");
  return {};
}

PsychEngine::~PsychEngine()
{
  const auto shutdownResult = Shutdown();
  if (!shutdownResult) {
    CORE_LOG_ERROR("PsychEngine shutdown failed");
  }
}

Expected<void, errors::EngineError> PsychEngine::Shutdown()
{
  CORE_PROFILE_FUNCTION();
  CORE_PROFILE_SCOPE("PsychEngine::Shutdown");

  if (!m_Initialized && !m_LayerStack && !m_Window) {
    return {};
  }

  m_Running = false;
  m_LayerStack.reset();
  m_RenderTarget_.reset();
  m_Framebuffer_.reset();
  Renderer::Shutdown();
  m_Window.reset();

  m_Initialized            = false;
  s_Application            = nullptr;
  const auto projectResult = m_ProjectManager_.Shutdown(m_PathResolver_);
  util::EngineFilesystem::Shutdown();
  if (!projectResult) {
    return Unexpected(errors::EngineError::PsychEngineInitializationFailed);
  }

  const auto configResult = m_Config.Shutdown();
  if (!configResult) {
    return Unexpected(errors::EngineError::PsychEngineInitializationFailed);
  }

  CORE_LOG_INFO("PsychEngine Shutdown");
  return {};
}

void PsychEngine::HandleEvents()
{
  CORE_PROFILE_FUNCTION();
  Unique<Event> event = nullptr;
  while (m_EventHandler_.TryDequeueEvent(event)) {
    switch (event->GetEventType()) {
    case EventType::WindowClose:
      m_Running = false;
    }
    m_Window->HandleEvents(std::move(event));
  }
}

void PsychEngine::Run()
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

void PsychEngine::Stop()
{
  CORE_PROFILE_FUNCTION();
  m_Running = false;
  CORE_LOG_INFO("Exiting Main Application Loop");
}

PsychEngine& PsychEngine::Get()
{
  CORE_PROFILE_FUNCTION();
  CORE_ASSERT(s_Application != nullptr, "Application is NULLPTR during call to GET()");
  return *s_Application;
}

void PsychEngine::PushLayer(std::unique_ptr<Layer> layer)
{
  CORE_PROFILE_FUNCTION();
  m_LayerStack->PushLayer(std::move(layer));
}

void PsychEngine::PushOverlay(std::unique_ptr<Layer> layer)
{
  CORE_PROFILE_FUNCTION();
  m_LayerStack->PushOverlay(std::move(layer));
}

const PsychEngineConfig& PsychEngine::GetConfig() const
{
  CORE_ASSERT(s_Application, "PsychEngine does not exist yet");
  return m_Config;
}

const PsychEngineSpecification& PsychEngine::GetEngineSpecification() const { return GetConfig().GetPsychEngineSpec(); }

ProjectManager& PsychEngine::GetProjectManager() { return m_ProjectManager_; }

const ProjectManager& PsychEngine::GetProjectManager() const { return m_ProjectManager_; }

const FontManager& PsychEngine::GetFontLibrary() const { return m_FontLibrary_; }

const EventHandler& PsychEngine::GetEventHandler() const { return m_EventHandler_; }
EventHandler& PsychEngine::GetEventHandler() { return m_EventHandler_; }

Window& PsychEngine::GetWindow()
{
  CORE_ASSERT(m_Window != nullptr, "Call to: GetWindow() failed. m_Window is nullptr!");
  return *m_Window;
}

const Window& PsychEngine::GetWindow() const
{
  CORE_ASSERT(m_Window != nullptr, "Call to: GetWindow() failed. m_Window is nullptr!");
  return *m_Window;
}

Framebuffer& PsychEngine::GetFramebuffer()
{
  CORE_ASSERT(m_Framebuffer_ != nullptr, "Call to: GetFramebuffer() failed. m_Framebuffer_ is nullptr!");
  return *m_Framebuffer_;
}

const Framebuffer& PsychEngine::GetFramebuffer() const
{
  CORE_ASSERT(m_Framebuffer_ != nullptr, "Call to: GetFramebuffer() failed. m_Framebuffer_ is nullptr!");
  return *m_Framebuffer_;
}
} // namespace psych
