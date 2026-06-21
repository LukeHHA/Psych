#include "MainFileTree.h"
#include "Assets/Fonts/IconsFontAwesome7.h"
#include "FileSystem/FileSystem.h"
#include "imgui.h"

namespace ge::ui
{
void TraverseFileTreeNode(const util::FileNode* node)
{
  if (!node)
    return;

  if (node == nullptr) {
    return;
  }

  if (!node->isDir) {
    std::string label = std::string(ICON_FA_FILE) + " " + node->name + "##" +
                        node->path.string();
    ImGui::Selectable(label.c_str());
    return;
  }

  std::string label = std::string(ICON_FA_FOLDER) + " " + node->name + "##" +
                      node->path.string();

  ImGuiTreeNodeFlags flags =
      ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

  if (ImGui::TreeNodeEx(label.c_str(), flags)) {
    for (const auto& child : node->children) {
      TraverseFileTreeNode(child.get());
    }

    ImGui::TreePop();
  }
}

void MainFileTree(const util::FileNode* node)
{
  if (!node)
    return;

  ImGuiChildFlags child_flags = 0;
  child_flags |= ImGuiChildFlags_Borders;
  child_flags |= ImGuiChildFlags_ResizeX;

  ImGuiWindowFlags window_flags = 0;

  if (ImGui::BeginChild("FileView", ImVec2(250.0f, 0.0f), child_flags,
                        window_flags)) {
    for (const auto& child : node->children) {
      TraverseFileTreeNode(child.get());
    }
  }

  ImGui::EndChild();
}
} // namespace ge::ui
