#include "EditorLayer.h"
#include "Core/GameEngine.h"
#include "Debug/Instrumentor.h"
#include "UI/Modules/EditorMenuBar.h"
#include "UI/Modules/MainFileTree.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <filesystem>

#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API
#endif
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace ge
{
EditorLayer::EditorLayer() : Layer("ImGuiLayer") {}

void EditorLayer::OnAttach()
{
  CORE_PROFILE_FUNCTION();
  CORE_LOG_INFO("EditorLayer Attached");

  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad
  // Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable
  // Multi-Viewport / Platform Windows
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

  // Setup Platform/Renderer bindings
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding              = 0.5f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  SetDarkThemeColors();

  GameEngine& app = GameEngine::Get();
  GLFWwindow* window =
      static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

  float dpi_scale      = ImGui_ImplGlfw_GetContentScaleForWindow(window);
  float base_font_size = 18.0f;
  float font_size      = base_font_size * dpi_scale;

  ImFontConfig config;
  io.FontDefault = io.Fonts->AddFontFromFileTTF(
      "Editor/Assets/Fonts/JetBrainsMonoNerdFont-Regular.ttf", font_size,
      &config);

  style.ScaleAllSizes(dpi_scale);
}

void EditorLayer::OnDetach() {}

void EditorLayer::OnEvent(Event& e)
{
  // if (m_BlockEvents) {
  //   ImGuiIO& io = ImGui::GetIO();
  //   e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
  //   e.Handled |= e.IsInCategory(EventCategoryKeyboard) &
  //   io.WantCaptureKeyboard;
  // }
}

void EditorLayer::Begin() { CORE_PROFILE_FUNCTION(); }

void EditorLayer::End() { CORE_PROFILE_FUNCTION(); }

void EditorLayer::SetDarkThemeColors()
{
  auto& colors              = ImGui::GetStyle().Colors;
  colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

  // Headers
  colors[ImGuiCol_Header]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_HeaderActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Buttons
  colors[ImGuiCol_Button]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_ButtonActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Frame BG
  colors[ImGuiCol_FrameBg]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_FrameBgActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Tabs
  colors[ImGuiCol_Tab]                = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabHovered]         = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
  colors[ImGuiCol_TabActive]          = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
  colors[ImGuiCol_TabUnfocused]       = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

  // Title
  colors[ImGuiCol_TitleBg]          = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgActive]    = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
}

uint32_t EditorLayer::GetActiveWidgetID() const { return GImGui->ActiveId; }

void EditorLayer::OnImGuiRender()
{

  ImGuiWindowFlags window_flags = 0;
  window_flags |= ImGuiWindowFlags_MenuBar;
  window_flags |= ImGuiWindowFlags_NoCollapse;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
  window_flags |= ImGuiWindowFlags_NoResize;

  if (!ImGui::Begin("Editor", nullptr, window_flags)) {
    ImGui::End();
    return;
  }
  ui::WindowMenuBar();

  static std::unique_ptr<util::FileNode> FileTree;

  if (!FileTree) {
    FileTree =
        util::Filesystem::CreateDirectoryTree(std::filesystem::current_path());
  }

  ui::MainFileTree(FileTree.get());

  ImGui::End();
}

void EditorLayer::OnRender() {}
void EditorLayer::OnUpdate(float ts) {}

} // namespace ge
