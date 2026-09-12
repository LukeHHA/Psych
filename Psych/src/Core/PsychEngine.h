
/**************************************************************************/
/*  PsychEngine.h                                                         */
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

#pragma once

#include "Config/DefaultConfig.h"
#include "Config/PsychEngineConfig.h"
#include "Core/Core.h"
#include "Core/Window.h"
#include "Debug/Assert.h"
#include "Errors/Errors.h"
#include "Events/EventHandler.h"
#include "Fonts/FontManager.h"
#include "Layers/LayerStack.h"
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
  [[nodiscard]] const PsychEngineSpecification& GetConfig() const;
  [[nodiscard]] PsychEngineSpecification& GetConfig();
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
  FontManager m_FontLibrary_;
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
