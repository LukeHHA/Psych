#include "FileViewer.h"
#include "FileSystem/FileSystem.h"
#include <filesystem>
#include <imgui.h>

namespace psych::ui
{
Unique<FileViewerPanel> FileViewerPanel::Create() { return CreateUnique<FileViewerPanel>(); }

std::string FileViewerPanel::GetID() { return "FileViewerPanel"; }

void FileViewerPanel::OnImGuiRender()
{
  auto fileContent = Filesystem::TryReadFile(m_Path_);
  if (!fileContent) {
    fileContent = "";
  }

  if (!ImGui::Begin("File Viewer")) {
    ImGui::End();
    return;
  }

  ImGui::BeginChild("Content", ImVec2{0, 0}, 1);
  ImGui::TextUnformatted(fileContent.value().c_str());
  ImGui::EndChild();

  ImGui::End();
}

void FileViewerPanel::OnUpdate() {}

void FileViewerPanel::SetFilePath(const std::filesystem::path& path) { m_Path_ = path; }
} // namespace psych::ui
