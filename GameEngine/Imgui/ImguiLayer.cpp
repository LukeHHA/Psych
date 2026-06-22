#include "ImguiLayer.h"
#include "Core/GameEngine.h"
#include "Debug/Instrumentor.h"
#include "imgui/imgui.h"
#include <filesystem>
#include <string>

#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API
#endif
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace ge
{
namespace
{
void ApplyEditorTheme()
{
  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
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

void LoadEditorFont(const EditorUISpec& spec)
{
  ImGuiIO& io = ImGui::GetIO();

  const float dpiScale =
      io.DisplayFramebufferScale.x > 0.0f ? io.DisplayFramebufferScale.x : 1.0f;
  const float fontSize = spec.FontSize * dpiScale;

  ImGuiStyle& style    = ImGui::GetStyle();
  style.ScaleAllSizes(dpiScale);

  if (!spec.FontPath.empty() && std::filesystem::exists(spec.FontPath)) {
    ImFontConfig config;
    io.FontDefault =
        io.Fonts->AddFontFromFileTTF(spec.FontPath.string().c_str(),
                                     fontSize,
                                     &config);
  }

  if (io.FontDefault == nullptr) {
    io.FontDefault = io.Fonts->AddFontDefault();
  }
}

void ConfigureEditorUIRuntime(const EditorUISpec& spec)
{
  ImGuiIO& io = ImGui::GetIO();

  if (spec.EnableKeyboardNavigation) {
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  }

  if (spec.EnableGamepadNavigation) {
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  }

  if (spec.EnableDocking) {
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  }

  if (spec.EnableMultiViewports) {
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigDpiScaleViewports = true;
  }

  ApplyEditorTheme();
  LoadEditorFont(spec);
}
} // namespace

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

void ImGuiLayer::OnAttach()
{
  CORE_PROFILE_FUNCTION();
  CORE_LOG_INFO("IMGUI Layer atached");

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  GameEngine& app = GameEngine::Get();
  ConfigureEditorUIRuntime(app.GetEngineSpecification().EditorUI);

  GLFWwindow* window =
      static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

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
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGui::DockSpaceOverViewport(0,
                                 ImGui::GetMainViewport(),
                                 ImGuiDockNodeFlags_PassthruCentralNode);
  }
}

void ImGuiLayer::End()
{
  CORE_PROFILE_FUNCTION();

  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void ImGuiLayer::OnImGuiRender() {}
} // namespace ge
