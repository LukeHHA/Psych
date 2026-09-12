#include "MainFileTree.h"
#include "Assets/Fonts/IconsFontAwesome7.h"
#include "FileSystem/FileSystem.h"
#include <imgui.h>
#include <utility>

namespace psych::ui
{
Unique<MainFileTreePanel> MainFileTreePanel::Create() { return CreateUnique<MainFileTreePanel>(); }

std::string MainFileTreePanel::GetID() { return "MainFileTreePanel"; }

void MainFileTreePanel::OnImGuiRender()
{
  if (!ImGui::Begin("File Tree")) {
    ImGui::End();
    return;
  }

  ImGuiChildFlags childFlags = 0;
  childFlags |= ImGuiChildFlags_AutoResizeY;
  childFlags |= ImGuiChildFlags_ResizeX;

  if (ImGui::BeginChild("FileView", ImVec2(250.0f, 0.0f), childFlags)) {
    DrawFileTreeNode(m_RootNode_);
  }

  ImGui::EndChild();
  ImGui::End();
}

void MainFileTreePanel::OnUpdate() {}

void MainFileTreePanel::SetSelectedCallbackFn(SelectedCallbackFn fn) { m_SelectedCallbacks_.push_back(std::move(fn)); }

void MainFileTreePanel::SetRootNode(const FileNode* node) { m_RootNode_ = node; }

void MainFileTreePanel::DrawFileTreeNode(const FileNode* node)
{
  if (node == nullptr) {
    return;
  }

  if (!node->isDir) {
    std::string label = std::string(ICON_FA_FILE) + " " + node->name + "##" + node->path.string();
    if (ImGui::Selectable(label.c_str())) {
      m_CurrentSelected_ = node->path;
      for (const auto& callback : m_SelectedCallbacks_) {
        callback(m_CurrentSelected_);
      }
    }
    return;
  }

  const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DrawLinesFull;
  std::string label              = std::string(ICON_FA_FOLDER) + " " + node->name + "##" + node->path.string();

  if (ImGui::TreeNodeEx(label.c_str(), flags)) {
    for (const auto& child : node->children) {
      DrawFileTreeNode(child.get());
    }

    ImGui::TreePop();
  }
}

const EnginePath::Path& MainFileTreePanel::GetCurrentSelected() const { return m_CurrentSelected_; }
} // namespace psych::ui
