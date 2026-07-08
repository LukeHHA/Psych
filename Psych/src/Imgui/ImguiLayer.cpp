 
/**************************************************************************/
/*  ImguiLayer.cpp                                                        */                                                            
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

#include "ImguiLayer.h"
#include "Core/PsychEngine.h"
#include "Debug/Instrumentor.h"
#include "FileSystem/EngineFilesystem.h"
#include "imgui/imgui.h"
#include <limits>
#include <string>

#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API
#endif
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace psych
{
void ImGuiLayer::ApplyEditorTheme()
{
  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  if ((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
    style.WindowRounding              = 0.5f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  auto& colors                        = style.Colors;
  colors[ImGuiCol_WindowBg]           = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

  colors[ImGuiCol_Header]             = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_HeaderHovered]      = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_HeaderActive]       = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  colors[ImGuiCol_Button]             = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_ButtonHovered]      = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_ButtonActive]       = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  colors[ImGuiCol_FrameBg]            = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_FrameBgHovered]     = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_FrameBgActive]      = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  colors[ImGuiCol_Tab]                = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabHovered]         = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
  colors[ImGuiCol_TabActive]          = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
  colors[ImGuiCol_TabUnfocused]       = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

  colors[ImGuiCol_TitleBg]            = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgActive]      = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgCollapsed]   = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
}

void ImGuiLayer::LoadEditorFont()
{
  ImGuiIO& io          = ImGui::GetIO();

  const float dpiScale = io.DisplayFramebufferScale.x > 0.0f ? io.DisplayFramebufferScale.x : 1.0f;
  const float fontSize = m_EditorSpec_.FontSize * dpiScale;

  ImGuiStyle& style    = ImGui::GetStyle();
  style.ScaleAllSizes(dpiScale);

  auto fontLibrary = PsychEngine::Get().GetFontLibrary();
  auto font        = fontLibrary.GetDefaultFont();

  if (font.IsValid()) {
    CORE_ASSERT(font.size() <= static_cast<std::size_t>(std::numeric_limits<int>::max()), "Embedded font is too large")

    ImFontConfig config{};
    config.FontDataOwnedByAtlas = false;

    io.FontDefault              = io.Fonts->AddFontFromMemoryTTF(font.data(), static_cast<int>(font.size()), fontSize, &config);
  }

  if (io.FontDefault == nullptr) {
    io.FontDefault = io.Fonts->AddFontDefault();
  }
}

void ImGuiLayer::ConfigureEditorUIRuntime()
{
  ImGuiIO& io = ImGui::GetIO();

  if (m_EditorSpec_.EnableKeyboardNavigation) {
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  }

  if (m_EditorSpec_.EnableGamepadNavigation) {
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  }

  if (m_EditorSpec_.EnableDocking) {
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  }

  if (m_EditorSpec_.EnableMultiViewports) {
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigDpiScaleViewports = true;
  }

  if (m_EditorSpec_.ImGuiINIPath.find("://") != std::string::npos) {
    const auto iniPath = util::EngineFilesystem::TryResolve(m_EditorSpec_.ImGuiINIPath);
    if (iniPath) {
      m_EditorSpec_.ImGuiINIPath = iniPath.value().string();
    }
  }
  io.IniFilename = m_EditorSpec_.ImGuiINIPath.c_str();

  ApplyEditorTheme();
  LoadEditorFont();
}

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

void ImGuiLayer::OnAttach()
{
  CORE_PROFILE_FUNCTION();
  CORE_LOG_INFO("IMGUI Layer atached");

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  PsychEngine& app = PsychEngine::Get();
  m_EditorSpec_    = app.GetConfig().GetPsychEngineSpec().EditorUI;
  ConfigureEditorUIRuntime();

  auto* window = app.GetWindow().GetNativeWindow();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
void ImGuiLayer::OnEvent(Event& e) {}

void ImGuiLayer::Begin()
{
  CORE_PROFILE_FUNCTION();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  // ImGuizmo::BeginFrame();

  const ImGuiIO& io = ImGui::GetIO();
  if ((io.ConfigFlags & ImGuiConfigFlags_DockingEnable) != 0) {
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
  }
}

void ImGuiLayer::End()
{
  CORE_PROFILE_FUNCTION();

  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  ImGuiIO& io = ImGui::GetIO();
  if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void ImGuiLayer::OnImGuiRender() {}
} // namespace psych
