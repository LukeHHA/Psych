#include "FileViewer.h"
#include "FileSystem/FileSystem.h"
#include <filesystem>
#include <imgui.h>

namespace ge::ui
{
void FileViewer(const std::filesystem::path& path)
{

  auto fileContent = util::Filesystem::TryReadFile(path);
  if (!fileContent) {
    return;
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
} // namespace ge::ui
