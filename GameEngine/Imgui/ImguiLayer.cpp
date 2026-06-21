#include "ImguiLayer.h"
#include "Core/GameEngine.h"
#include "Debug/Instrumentor.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API
#endif
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace ge
{
ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

void ImGuiLayer::OnAttach()
{
  CORE_PROFILE_FUNCTION();
  CORE_LOG_INFO("IMGUI Layer atached");

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  GameEngine& app = GameEngine::Get();
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
  ImGuiViewport* viewport = ImGui::GetMainViewport();

  ImGui::SetNextWindowPos(viewport->WorkPos, ImGuiCond_Always);

  ImGui::SetNextWindowSize(viewport->WorkSize, ImGuiCond_Always);

  ImGui::SetNextWindowViewport(viewport->ID);
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
