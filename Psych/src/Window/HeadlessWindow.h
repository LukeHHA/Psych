 
/**************************************************************************/
/*  HeadlessWindow.h                                                      */                                                            
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

#include "Core/Core.h"
#include "Core/Window.h"
#include "Core/glad_glfw_incl.h"
#include "Errors/Errors.h"
#include "Events/Event.h"
#include "Renderer/RendererContext.h"
#include "expected.h"

#include <functional>
#include <string>

namespace psych
{
class HeadlessWindow : public Window
{
public:
  using EventCallbackFn                            = std::function<void(Event&)>;

  HeadlessWindow()                                 = default;
  HeadlessWindow(const HeadlessWindow&)            = delete;
  HeadlessWindow(HeadlessWindow&&)                 = delete;
  HeadlessWindow& operator=(const HeadlessWindow&) = delete;
  HeadlessWindow& operator=(HeadlessWindow&&)      = delete;
  ~HeadlessWindow() override;

  Expected<void, errors::WindowError> Init(const std::string& title, unsigned int width, unsigned int height, EventHandler& eventHandler) override;
  Expected<void, errors::WindowError> Shutdown() override;
  void PollEvents() override {}
  [[nodiscard]] unsigned int GetWidth() const override { return m_Data.Width; }
  [[nodiscard]] unsigned int GetHeight() const override { return m_Data.Height; }
  [[nodiscard]] GLFWwindow* GetNativeWindow() const override { return m_Window.get(); }
  void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
  void OnUpdate() override;
  void SetVSync(bool enabled) override;
  void HandleEvents(Unique<Event> event) override;
  [[nodiscard]] bool IsVSync() const override;

private:
  Unique<RendererContext> m_RendererContext;
  UniqueGLFWwindow m_Window;

  struct WindowData {
    std::string Title;
    unsigned int Width, Height;
    bool VSync;

    EventCallbackFn EventCallback;
  };

  WindowData m_Data;
};

} // namespace psych
