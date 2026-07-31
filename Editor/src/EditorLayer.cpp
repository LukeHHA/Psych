#include "EditorLayer.h"
#include "Core/Core.h"
#include "Core/PsychEngine.h"
#include "Debug/Instrumentor.h"
#include "FileSystem/FileSystem.h"
#include "Renderer/Buffer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/VertexArray.h"
#include "UI/Modules/EditorMenuBar.h"
#include "UI/Modules/FileViewer.h"
#include "UI/Modules/MainFileTree.h"
#include "UI/Modules/Viewport.h"
#include "Util/Time.h"
#include <cstdint>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace psych
{
EditorLayer::EditorLayer() : Layer("EditorLayer") {}

void EditorLayer::OnAttach()
{
  CORE_PROFILE_FUNCTION();
  CORE_LOG_INFO("EditorLayer Attached");

  static const float cubeVertices[] = {
      // position           // color
      -0.5f, -0.5f, -0.5f, 1.0f, 0.1f, 0.1f, 0.5f, -0.5f, -0.5f, 0.1f, 1.0f, 0.1f, 0.5f, 0.5f, -0.5f, 0.1f, 0.4f, 1.0f, -0.5f, 0.5f, -0.5f, 1.0f, 0.9f, 0.1f,
      -0.5f, -0.5f, 0.5f,  1.0f, 0.1f, 0.8f, 0.5f, -0.5f, 0.5f,  0.1f, 1.0f, 0.9f, 0.5f, 0.5f, 0.5f,  0.9f, 0.4f, 1.0f, -0.5f, 0.5f, 0.5f,  1.0f, 0.5f, 0.1f,
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

  auto shaderResult = Shader::Create("engine://Editor/Assets/Shaders/editor_cube.vert.glsl", "engine://Editor/Assets/Shaders/editor_cube.frag.glsl", "EditorCube");
  if (!shaderResult) {
    CORE_LOG_ERROR("Failed to create editor cube shader");
  } else {
    m_CubeShader_ = shaderResult.value();
  }

  auto rendererAPI = RendererAPI::Create();
  if (!rendererAPI) {
    CORE_LOG_ERROR("Failed to create editor renderer API");
  } else {
    m_RendererAPI_ = std::move(rendererAPI.value());
  }

  m_PanelManager_.AddPanel<ui::EditorMenuBarPanel>();

  auto* fileTree = m_PanelManager_.AddPanel<ui::MainFileTreePanel>();
  fileTree->SetSelectedCallbackFn([this](const std::filesystem::path& path) { OpenFile(path); });

  auto fileTreeResult = Filesystem::TryCreateDirectoryTree(Filesystem::Current_Path());
  if (!fileTreeResult) {
    CORE_LOG_ERROR("Failed to create editor file tree");
  } else {
    m_FileTreeRoot_ = std::move(fileTreeResult.value());
    fileTree->SetRootNode(m_FileTreeRoot_.get());
  }

  m_PanelManager_.AddPanel<ui::FileViewerPanel>();
  m_PanelManager_.AddPanel<ui::ViewportPanel>();
  m_ProjectWindowPanel_ = ui::ProjectWindowPanel::Create();
}

void EditorLayer::OnDetach()
{
  m_RendererAPI_.reset();
  m_CubeShader_.reset();
  m_CubeVertexArray_.reset();
  m_ProjectWindowPanel_.reset();
}

void EditorLayer::OnEvent(Event& e)
{
  if (m_ShowNewProjectWindow_ && m_ProjectWindowPanel_ != nullptr) {
    m_ProjectWindowPanel_->OnEvent(e);
  } else {
    m_PanelManager_.OnEvent(e);
  }

  // if (m_BlockEvents) {
  //   ImGuiIO& io = ImGui::GetIO();
  //   e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
  //   e.Handled |= e.IsInCategory(EventCategoryKeyboard) &
  //   io.WantCaptureKeyboard;
  // }
}

void EditorLayer::Begin() { CORE_PROFILE_FUNCTION(); }

void EditorLayer::End() { CORE_PROFILE_FUNCTION(); }

void EditorLayer::OnImGuiRender()
{
  if (m_ShowNewProjectWindow_ && m_ProjectWindowPanel_ != nullptr) {
    m_ProjectWindowPanel_->OnImGuiRender();
  } else {
    m_PanelManager_.OnImGuiRender();
  }
}

void EditorLayer::OnRender()
{
  if (!m_CubeVertexArray_ || !m_CubeShader_ || !m_RendererAPI_) {
    return;
  }

  const auto& framebuffer    = PsychEngine::Get().GetFramebuffer();
  const float aspect         = framebuffer.GetHeight() > 0 ? static_cast<float>(framebuffer.GetWidth()) / static_cast<float>(framebuffer.GetHeight()) : 16.0F / 9.0F;

  const glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
  const glm::mat4 view       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
  const glm::mat4 model      = glm::rotate(glm::mat4(1.0f), m_CubeRotation_, glm::normalize(glm::vec3(0.4f, 1.0f, 0.2f)));

  m_CubeShader_->Bind();
  m_CubeShader_->SetMat4("u_MVP", projection * view * model);
  m_RendererAPI_->DrawIndexed(m_CubeVertexArray_, 36);
}

void EditorLayer::OnUpdate(float ts)
{
  if (m_ShowNewProjectWindow_ && m_ProjectWindowPanel_ != nullptr) {
    m_ProjectWindowPanel_->OnUpdate();
  } else {
    m_PanelManager_.OnUpdate();
  }
  m_CubeRotation_ += Time::DeltaTime();
}

void EditorLayer::OpenFile(const std::filesystem::path& path)
{
  auto* fileViewer = m_PanelManager_.GetPanel<ui::FileViewerPanel>();
  if (fileViewer != nullptr) {
    fileViewer->SetFilePath(path);
  }
}

} // namespace psych
