#include "EditorLayer.h"
#include "Core/GameEngine.h"
#include "Debug/Instrumentor.h"
#include "Renderer/Buffer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/VertexArray.h"
#include "UI/Modules/EditorMenuBar.h"
#include "UI/Modules/MainFileTree.h"
#include "Util/Time.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <cstdint>
#include <filesystem>

namespace ge
{
EditorLayer::EditorLayer() : Layer("EditorLayer") {}

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

  float dpi_scale =
      io.DisplayFramebufferScale.x > 0.0f ? io.DisplayFramebufferScale.x : 1.0f;
  float base_font_size = 18.0f;
  float font_size      = base_font_size * dpi_scale;

  ImFontConfig config;
  io.FontDefault = io.Fonts->AddFontFromFileTTF(
      "Editor/Assets/Fonts/JetBrainsMonoNerdFont-Regular.ttf",
      font_size,
      &config);

  style.ScaleAllSizes(dpi_scale);

  static const float cubeVertices[] = {
      // position           // color
      -0.5f, -0.5f, -0.5f, 1.0f, 0.1f,  0.1f,  0.5f, -0.5f, -0.5f, 0.1f,
      1.0f,  0.1f,  0.5f,  0.5f, -0.5f, 0.1f,  0.4f, 1.0f,  -0.5f, 0.5f,
      -0.5f, 1.0f,  0.9f,  0.1f, -0.5f, -0.5f, 0.5f, 1.0f,  0.1f,  0.8f,
      0.5f,  -0.5f, 0.5f,  0.1f, 1.0f,  0.9f,  0.5f, 0.5f,  0.5f,  0.9f,
      0.4f,  1.0f,  -0.5f, 0.5f, 0.5f,  1.0f,  0.5f, 0.1f,
  };

  static const uint32_t cubeIndices[] = {
      0, 1, 2, 2, 3, 0, // back
      4, 5, 6, 6, 7, 4, // front
      4, 0, 3, 3, 7, 4, // left
      1, 5, 6, 6, 2, 1, // right
      3, 2, 6, 6, 7, 3, // top
      4, 5, 1, 1, 0, 4  // bottom
  };

  auto vertexBuffer = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
  vertexBuffer->SetLayout(VertexFormatID::PC);

  auto indexBuffer   = IndexBuffer::Create(cubeIndices, 36);

  m_CubeVertexArray_ = VertexArray::Create();
  m_CubeVertexArray_->AddVertexBuffer(vertexBuffer);
  m_CubeVertexArray_->AddIndexBuffer(indexBuffer);

  m_CubeShader_  = Shader::Create("data/Shaders/editor_cube.vert.glsl",
                                  "data/Shaders/editor_cube.frag.glsl",
                                  "EditorCube");
  m_RendererAPI_ = RendererAPI::Create();
}

void EditorLayer::OnDetach()
{
  m_RendererAPI_.reset();
  m_CubeShader_.reset();
  m_CubeVertexArray_.reset();
}

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
  ImGui::SameLine();

  ImGuiWindowFlags viewportWindowFlags = 0;
  viewportWindowFlags |= ImGuiWindowFlags_NoScrollbar;
  viewportWindowFlags |= ImGuiWindowFlags_NoScrollWithMouse;

  if (ImGui::BeginChild("Viewport",
                        ImVec2(0.0f, 0.0f),
                        ImGuiChildFlags_Borders,
                        viewportWindowFlags)) {
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    if (viewportSize.x > 0.0f && viewportSize.y > 0.0f) {
      auto& framebuffer             = GameEngine::Get().GetFramebuffer();

      const uint32_t viewportWidth  = static_cast<uint32_t>(viewportSize.x);
      const uint32_t viewportHeight = static_cast<uint32_t>(viewportSize.y);

      if (viewportWidth != m_ViewportWidth_ ||
          viewportHeight != m_ViewportHeight_) {
        framebuffer.Resize(viewportWidth, viewportHeight);
        m_ViewportWidth_  = viewportWidth;
        m_ViewportHeight_ = viewportHeight;
      }

      const ImTextureID framebufferTexture =
          static_cast<ImTextureID>(framebuffer.GetColorAttachmentID());

      ImGui::Image(framebufferTexture,
                   viewportSize,
                   ImVec2(0.0f, 1.0f),
                   ImVec2(1.0f, 0.0f));
    }
  }
  ImGui::EndChild();

  ImGui::End();
}

void EditorLayer::OnRender()
{
  if (!m_CubeVertexArray_ || !m_CubeShader_ || !m_RendererAPI_) {
    return;
  }

  const float aspect = m_ViewportHeight_ > 0
                           ? static_cast<float>(m_ViewportWidth_) /
                                 static_cast<float>(m_ViewportHeight_)
                           : 16.0f / 9.0f;

  const glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
  const glm::mat4 view =
      glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
  const glm::mat4 model =
      glm::rotate(glm::mat4(1.0f),
                  m_CubeRotation_,
                  glm::normalize(glm::vec3(0.4f, 1.0f, 0.2f)));

  m_CubeShader_->Bind();
  m_CubeShader_->SetMat4("u_MVP", projection * view * model);
  m_RendererAPI_->DrawIndexed(m_CubeVertexArray_, 36);
}

void EditorLayer::OnUpdate(float ts) { m_CubeRotation_ += Time::DeltaTime(); }

} // namespace ge
