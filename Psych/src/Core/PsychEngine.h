#pragma once

#include "Config/PsychEngineConfig.h"
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
#include "expected.h"

namespace psych
{
class PsychEngine
{
public:
  explicit PsychEngine();
  virtual ~PsychEngine();
  CORE_NO_COPY_NO_MOVE(PsychEngine);

  Expected<void, errors::EngineError> Init();
  Expected<void, errors::EngineError> Shutdown();
  void Run();
  void Stop();
  void HandleEvents();
  void PushLayer(std::unique_ptr<Layer> layer);
  void PushOverlay(std::unique_ptr<Layer> layer);

  /// getters for member objects
  [[nodiscard]] const PsychEngineConfig& GetConfig() const;
  [[nodiscard]] const PsychEngineSpecification& GetEngineSpecification() const;
  [[nodiscard]] ProjectManager& GetProjectManager();
  [[nodiscard]] const ProjectManager& GetProjectManager() const;
  [[nodiscard]] const FontManager& GetFontLibrary() const;

  /// getters for ptrs
  [[nodiscard]] const EventHandler& GetEventHandler() const;
  [[nodiscard]] EventHandler& GetEventHandler();
  [[nodiscard]] Window& GetWindow();
  [[nodiscard]] const Window& GetWindow() const;
  [[nodiscard]] const Framebuffer& GetFramebuffer() const;
  [[nodiscard]] Framebuffer& GetFramebuffer();
  [[nodiscard]] static PsychEngine& Get();

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
  /// default constructed objects - reverse destruction order
  PsychEngineConfig m_Config;
  util::PathResolver m_PathResolver_;
  FontManager m_FontLibrary_;
  ProjectManager m_ProjectManager_;
  EventHandler m_EventHandler_;

  /// engine instance
  static PsychEngine* s_Application;

  /// polymorphic classes
  Unique<LayerStack> m_LayerStack;
  Unique<Window> m_Window;
  Unique<Framebuffer> m_Framebuffer_;
  Unique<RenderTarget> m_RenderTarget_;

  // member vars
  bool m_Initialized = false;
  bool m_Running     = false;
};

/// User defined - extern declartion in entrypoint.h
Expected<Unique<PsychEngine>, errors::EngineError> CreatePsychEngine();
} // namespace psych
