#include "MainFileTree.h"
#include "Assets/Fonts/IconsFontAwesome7.h"
#include "FileSystem/FileSystem.h"
#include <imgui.h>

namespace psych::ui
{
void TraverseFileTreeNode(const util::FileNode* node, ImGuiTreeNodeFlags flags)
{
  if (node == nullptr) {
    return;
  }

  if (!node->isDir) {
    std::string label = std::string(ICON_FA_FILE) + " " + node->name + "##" + node->path.string();
    ImGui::Selectable(label.c_str());
    return;
  }

  std::string label = std::string(ICON_FA_FOLDER) + " " + node->name + "##" + node->path.string();

  if (ImGui::TreeNodeEx(label.c_str(), flags)) {
    for (const auto& child : node->children) {
      TraverseFileTreeNode(child.get(), flags);
    }

    ImGui::TreePop();
  }
}

void MainFileTree(const util::FileNode* node)
{
  if (node == nullptr) {
    return;
  }

  ImGuiChildFlags child_flags = 0;
  child_flags |= ImGuiChildFlags_AutoResizeY;
  child_flags |= ImGuiChildFlags_ResizeX;

  ImGuiWindowFlags window_flags = 0;
  ImGuiTreeNodeFlags flags      = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DrawLinesFull;

  if (ImGui::BeginChild("FileView", ImVec2(250.0f, 0.0f), child_flags, window_flags)) {
    if (!node->isDir) {
      std::string label = std::string(ICON_FA_FILE) + " " + node->name + "##" + node->path.string();
      ImGui::Selectable(label.c_str());
      return;
    }

    std::string label = std::string(ICON_FA_FOLDER) + " " + node->name + "##" + node->path.string();
    if (ImGui::TreeNodeEx(label.c_str(), flags)) {
      for (const auto& child : node->children) {
        TraverseFileTreeNode(child.get(), flags);
      }
      ImGui::TreePop();
    }
  }

  ImGui::EndChild();
}
} // namespace psych::ui
