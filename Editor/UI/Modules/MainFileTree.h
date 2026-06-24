#pragma once

#include "FileSystem/FileSystem.h"
#include <imgui.h>
namespace ge::ui
{
void TraverseFileTreeNode(const util::FileNode* node, ImGuiTreeNodeFlags flags);
void MainFileTree(const util::FileNode* node);
} // namespace ge::ui
