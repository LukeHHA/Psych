#include "EditorLayer.h"
#include "Core/Core.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "FileSystem/FileSystem.h"
#include "Logging/Logging.h"
#include "UI/Modules/EditorMenuBar.h"
#include "UI/Modules/FileViewer.h"
#include "UI/Modules/MainFileTree.h"
#include "UI/Modules/PopupModal.h"
#include "UI/Modules/Viewport.h"
#include "Util/Time.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace psych
{
EditorLayer::EditorLayer() : Layer("EditorLayer") {}

void EditorLayer::OnAttach()
{
  CORE_PROFILE_FUNCTION();
  CORE_LOG_INFO("EditorLayer Attached");

  auto pm_result = m_ProjectManager_.Init();
  CORE_ASSERT(pm_result, "Failed to initialise project manager")

  m_PanelManager_.AddPanel<ui::EditorMenuBarPanel>();

  auto* fileTree = m_PanelManager_.AddPanel<ui::MainFileTreePanel>();
  fileTree->SetSelectedCallbackFn([this](const EnginePath::Path& path) { OpenFile(path); });

  m_PanelManager_.AddPanel<ui::FileViewerPanel>();
  m_PanelManager_.AddPanel<ui::ViewportPanel>();
  m_ProjectWindowPanel_ = ui::ProjectWindowPanel::Create();
  m_ProjectWindowPanel_->SetProjectSelectedCallbackFn([this](const std::filesystem::path& path) { return OpenProject(path); });
}

void EditorLayer::OnDetach()
{
  m_ProjectWindowPanel_.reset();
  auto pm_result = m_ProjectManager_.Shutdown();
  CORE_ASSERT(pm_result, "Project manager failed to shutdown")
}

void EditorLayer::OnEvent(Event* e)
{
  if (e->GetEventType() == EventType::OpenProjectWindow) {
    m_ShowNewProjectWindow_ = true;
    CORE_LOG_INFO("Project event was handled");
  }

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

void EditorLayer::OnRender() {}

void EditorLayer::OnUpdate(float ts)
{
  if (m_ShowNewProjectWindow_ && m_ProjectWindowPanel_ != nullptr) {
    m_ProjectWindowPanel_->OnUpdate();
  } else {
    m_PanelManager_.OnUpdate();
  }
}

void EditorLayer::OpenFile(const EnginePath::Path& path)
{
  auto* fileViewer = m_PanelManager_.GetPanel<ui::FileViewerPanel>();
  if (fileViewer != nullptr) {
    fileViewer->SetFilePath(path);
  }
}

bool EditorLayer::OpenProject(const std::filesystem::path& path)
{
  if (!Filesystem::DirExists(path)) {
    CORE_LOG_ERROR("Project path is not a directory: {}", path.string());
    return false;
  }

  if (!Filesystem::FileExists(path / "project.xml")) {
    ui::PopupModal("Create new project?");
  }

  const auto projectResult = m_ProjectManager_.OpenProject(path);
  if (!projectResult) {
    CORE_LOG_ERROR("Failed to open project: {}", path.string());
    return false;
  }

  const auto assetRoot = m_ProjectManager_.GetActiveProject().GetAssetRootPath();
  auto fileTreeResult  = Filesystem::TryCreateDirectoryTree(assetRoot);
  if (!fileTreeResult) {
    CORE_LOG_ERROR("Failed to open project asset directory: {}", assetRoot.string());
    return false;
  }

  auto* fileTree = m_PanelManager_.GetPanel<ui::MainFileTreePanel>();
  if (fileTree == nullptr) {
    CORE_LOG_ERROR("Failed to find the project file tree panel");
    return false;
  }

  // TODO: Consider moving the whole file tree root into the filetree panel
  //  makes more sense for the filetree to own that knowledge
  m_FileTreeRoot_ = std::move(fileTreeResult.value());
  fileTree->SetRootNode(m_FileTreeRoot_.get());
  m_ShowNewProjectWindow_ = false;

  CORE_LOG_INFO("Opened project directory: {}", path.string());
  return true;
}

} // namespace psych
