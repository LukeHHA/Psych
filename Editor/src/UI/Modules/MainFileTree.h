#pragma once

#include "FileSystem/FileSystem.h"
#include <imgui.h>
namespace psych::ui
{
void TraverseFileTreeNode(const util::FileNode* node, ImGuiTreeNodeFlags flags);
void MainFileTree(const util::FileNode* node);
} // namespace psych::ui
