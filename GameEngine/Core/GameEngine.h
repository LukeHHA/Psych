#pragma once

#include "Config/GameEngineConfig.h"
#include "Core/Core.h"
#include "Core/Window.h"
#include "Debug/Assert.h"
#include "Errors/Errors.h"
#include "Events/EventHandler.h"
#include "FileSystem/PathResolver.h"
#include "Fonts/FontManager.h"
#include "Layers/LayerStack.h"
#include "Project/ProjectManager.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderTarget.h"
#include "ge_expected"

namespace ge
{
class GameEngine
{
public:
  explicit GameEngine();
  virtual ~GameEngine();
  CORE_NO_COPY_NO_MOVE(GameEngine);

  Expected<void, errors::EngineError> Init();
  Expected<void, errors::EngineError> Shutdown();
  void Run();
  void Stop();
  void HandleEvents();
  void PushLayer(std::unique_ptr<Layer> layer);
  void PushOverlay(std::unique_ptr<Layer> layer);
  [[nodiscard]] const GameEngineConfig& GetConfig() const;
  [[nodiscard]] const GameEngineSpecification& GetEngineSpecification() const;
  [[nodiscard]] ProjectManager& GetProjectManager();
  [[nodiscard]] const ProjectManager& GetProjectManager() const;
  [[nodiscard]] const FontManager& GetFontLibrary() const;
  [[nodiscard]] const Shared<EventHandler>& GetEventHandler() const;
  Window& GetWindow()
  {
    CORE_ASSERT(m_Window != nullptr, "Call to: GetWindow() failed. m_Window is nullptr!");
    return *m_Window;
  }
  Framebuffer& GetFramebuffer()
  {
    CORE_ASSERT(m_Framebuffer_ != nullptr, "Call to: GetFramebuffer() failed. m_Framebuffer_ is nullptr!");
    return *m_Framebuffer_;
  }
  static GameEngine& Get();

private:
  [[nodiscard]] LayerStack& Layers()
  {
    CORE_ASSERT(m_LayerStack, "LayerStack is nullptr");
    return *m_LayerStack;
  }
  [[nodiscard]] const LayerStack& Layers() const
  {
    CORE_ASSERT(m_LayerStack, "Layerstack is nullptr");
    return *m_LayerStack;
  }

private:
  GameEngineConfig m_Config;
  util::PathResolver m_PathResolver_;
  ProjectManager m_ProjectManager_;
  bool m_Initialized = false;
  bool m_Running     = false;
  static GameEngine* s_Application;
  Unique<LayerStack> m_LayerStack;
  Shared<Window> m_Window;
  Shared<EventHandler> m_EventHandler_;
  Shared<Framebuffer> m_Framebuffer_;
  FontManager m_FontLibrary_;
  Unique<RenderTarget> m_RenderTarget_;
};
Expected<Unique<GameEngine>, errors::EngineError> CreateGameEngine();
} // namespace ge
