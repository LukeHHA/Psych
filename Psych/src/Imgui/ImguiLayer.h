
/**************************************************************************/
/*  ImguiLayer.h                                                          */
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
#include "Layers/Layer.h"
namespace psych
{

class ImGuiLayer : public Layer
{
public:
  ImGuiLayer();
  ImGuiLayer(const ImGuiLayer&)            = default;
  ImGuiLayer(ImGuiLayer&&)                 = delete;
  ImGuiLayer& operator=(const ImGuiLayer&) = delete;
  ImGuiLayer& operator=(ImGuiLayer&&)      = delete;
  ~ImGuiLayer() override                   = default;

  void OnAttach() override;
  void OnDetach() override;
  void OnEvent(Event* e) override;
  void OnUpdate(float ts = 1) override {}
  void OnRender() override {}

  static void Begin();
  static void End();

  void OnImGuiRender() override;
  // void BlockEvents(bool block) { m_BlockEvents = block; }
  // void SetDarkThemeColors();
  // uint32_t GetActiveWidgetID() const;

private:
  void ConfigureEditorUIRuntime();
  void ApplyEditorTheme();
  void LoadEditorFont();

private:
  bool m_BlockEvents = true;
  EditorUISpec m_EditorSpec_;
};

} // namespace psych
