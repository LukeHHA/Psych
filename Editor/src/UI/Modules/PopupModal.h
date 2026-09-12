#pragma once

#include <imgui.h>
#include <string>
namespace psych::ui
{
bool PopupModal(const std::string& title)
{
  ImGui::OpenPopup("Delete?");

  // Always center this window when appearing
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

  if (bool result = ImGui::BeginPopupModal("Delete?", nullptr, ImGuiWindowFlags_AlwaysAutoResize); result) {
    ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!");
    ImGui::Separator();

    // static int unused_i = 0;
    // ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

    static bool dont_ask_me_next_time = false;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
    ImGui::PopStyleVar();

    if (ImGui::Button("OK", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
    return result;
  }
  return false;
}
} // namespace psych::ui
