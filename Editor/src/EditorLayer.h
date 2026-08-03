#pragma once

#include "Core/Core.h"
#include "FileSystem/FileSystem.h"
#include "Layers/Layer.h"
#include "PanelManager.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "UI/Modules/ProjectWindow.h"
#include <cstdint>
#include <filesystem>

namespace psych
{
class EditorLayer : public Layer
{
public:
  EditorLayer();
  EditorLayer(const EditorLayer&)            = delete;
  EditorLayer(EditorLayer&&)                 = delete;
  EditorLayer& operator=(const EditorLayer&) = delete;
  EditorLayer& operator=(EditorLayer&&)      = delete;
  ~EditorLayer() override                    = default;

  void OnAttach() override;
  void OnDetach() override;
  void OnEvent(Event& event) override;
  virtual void Begin();
  virtual void End();
  void OnUpdate(float ts = 1) override;
  void OnRender() override;
  void OnImGuiRender() override;

private:
  void OpenFile(const std::filesystem::path& path);
  bool OpenProject(const std::filesystem::path& path);

private:
  Shared<VertexArray> m_CubeVertexArray_;
  Shared<Shader> m_CubeShader_;
  Unique<RendererAPI> m_RendererAPI_;
  Unique<FileNode> m_FileTreeRoot_;
  float m_CubeRotation_ = 0.0f;

  PanelManager m_PanelManager_;
  Unique<ui::ProjectWindowPanel> m_ProjectWindowPanel_;

  bool m_ShowNewProjectWindow_ = true;
};
} // namespace psych
