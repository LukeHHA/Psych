#include "ProjectWindow.h"
#include "FileSystem/FileSystem.h"
#include "ImGui/misc/cpp/imgui_stdlib.cpp"
#include <iostream>

namespace psych
{
void ProjectWindow::OnImGuiRender()
{
  const ImGuiViewport* viewport = ImGui::GetMainViewport();

  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);

  const ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking;

  const ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_ElideLeft | ImGuiInputTextFlags_ReadOnly;

  if (ImGui::Begin("New Project", nullptr, flags)) {
    if (ImGui::Button("New Project")) {
      auto path = Filesystem::GetFileExplorer();
      std::cout << path << std::endl;
      m_ProjectPath_ = path.string();
    }
    ImGui::InputText("Project Path", &m_ProjectPath_, inputFlags);
  }
  ImGui::End();
}
} // namespace psych
