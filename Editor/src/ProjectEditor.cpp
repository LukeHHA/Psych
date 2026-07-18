#include "ProjectEditor.h"
#include <imgui.h>

namespace psych
{
ProjectEditor::ProjectEditor() : Layer("ProjectEditor") {}

void ProjectEditor::OnAttach() {}
void ProjectEditor::OnDetach() {}
void ProjectEditor::OnEvent(Event& event) {}
void ProjectEditor::OnUpdate(float ts) {}
void ProjectEditor::OnRender() {}
void ProjectEditor::Begin() {}
void ProjectEditor::End() {}
void ProjectEditor::OnImGuiRender()
{
  const ImGuiViewport* viewport = ImGui::GetMainViewport();

  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);

  const ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking;

  if (ImGui::Begin("New Project", nullptr, flags)) {
  }
  ImGui::End();
}
} // namespace psych
