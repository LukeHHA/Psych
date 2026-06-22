#pragma once

#include "Core/Core.h"
#include "Core/Window.h"
#include "Debug/Assert.h"
#include "Errors/Errors.h"
#include "Events/EventHandler.h"
#include "FileSystem/FileSystem.h"
#include "Layers/LayerStack.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderTarget.h"
#include "Renderer/RendererAPI.h"
#include "ge_expected"

namespace ge
{
struct EditorUISpec {
  bool EnableDocking            = true;
  bool EnableMultiViewports     = true;
  bool EnableKeyboardNavigation = true;
  bool EnableGamepadNavigation  = false;
  util::FilePath FontPath;
  float FontSize = 18.0f;
};

struct GameEngineSpecification {
  std::string Name             = "Application";
  RendererAPIType RenderingAPI = RendererAPIType::OPENGL;
  util::FilePath AssetBasePath;
  bool EnableEditorUI = false;
  EditorUISpec EditorUI;
};

class GameEngine
{
public:
  GameEngine(const GameEngineSpecification& specification = GameEngineSpecification());
  virtual ~GameEngine();
  CORE_NO_COPY_NO_MOVE(GameEngine);

  Expected<void, errors::EngineError> Init();
  void Run();
  void Stop();
  void HandleEvents();
  void PushLayer(std::unique_ptr<Layer> layer);
  void PushOverlay(std::unique_ptr<Layer> layer);
  const GameEngineSpecification& GetEngineSpecification();
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
  Shared<Window> m_Window;
  Shared<EventHandler> m_EventHandler_;
  Shared<Framebuffer> m_Framebuffer_;
  Unique<RenderTarget> m_RenderTarget_;
};
Expected<Unique<GameEngine>, errors::EngineError> CreateGameEngine(GameEngineSpecification& spec);
} // namespace ge
