#pragma once

#include "FileSystem/FileSystem.h"
#include <imgui.h>
namespace psych::ui
{
void TraverseFileTreeNode(const FileNode* node, ImGuiTreeNodeFlags flags);
void MainFileTree(const FileNode* node);
} // namespace psych::ui
