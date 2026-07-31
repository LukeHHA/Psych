#include "ProjectWindow.h"
#include "Core/PsychEngine.h"
#include "FileSystem/FileSystem.h"
#include "Project/Project.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <imgui.h>

#include "imgui/misc/cpp/imgui_stdlib.h"
#include <system_error>
#include <utility>

namespace psych::ui
{
ProjectWindowPanel::ProjectWindowPanel() { const auto& config = PsychEngine::Get().GetEngineSpecification(); }

Unique<ProjectWindowPanel> ProjectWindowPanel::Create() { return CreateUnique<ProjectWindowPanel>(); }

std::string ProjectWindowPanel::GetID() { return "ProjectWindowPanel"; }

void ProjectWindowPanel::OnImGuiRender()
{
  const ImGuiViewport* viewport = ImGui::GetMainViewport();

  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);

  const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                 ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus;

  if (ImGui::Begin("New Project", nullptr, flags)) {
    const ImVec2 availableSize = ImGui::GetContentRegionAvail();
    const ImVec2 cardSize{
        std::max(320.0F, std::min(1080.0F, availableSize.x - 48.0F)),
        std::max(360.0F, std::min(680.0F, availableSize.y - 48.0F)),
    };

    ImGui::SetCursorPos(ImVec2(std::max(ImGui::GetStyle().WindowPadding.x, (ImGui::GetWindowWidth() - cardSize.x) * 0.5F),
                               std::max(ImGui::GetStyle().WindowPadding.y, (ImGui::GetWindowHeight() - cardSize.y) * 0.5F)));

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(24.0F, 22.0F));

    if (ImGui::BeginChild("ProjectBrowserCard", cardSize, ImGuiChildFlags_Borders, ImGuiWindowFlags_NoScrollbar)) {
      const ImVec4 accentColour = ImGui::GetStyleColorVec4(ImGuiCol_CheckMark);
      ImGui::TextColored(accentColour, "PSYCH");
      ImGui::SameLine();
      ImGui::TextDisabled("/ PROJECT BROWSER");
      ImGui::Dummy(ImVec2(0.0F, 4.0F));
      ImGui::TextUnformatted("Start something new or continue a recent project.");
      ImGui::Dummy(ImVec2(0.0F, 12.0F));
      ImGui::Separator();
      ImGui::Dummy(ImVec2(0.0F, 12.0F));

      if (ImGui::BeginTable("ProjectBrowserLayout", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoSavedSettings)) {
        ImGui::TableSetupColumn("Recent", ImGuiTableColumnFlags_WidthStretch, 1.35F);
        ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthStretch, 1.0F);
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
        if (ImGui::BeginChild("RecentProjectsCard", ImVec2(0.0F, 0.0F), ImGuiChildFlags_Borders)) {
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::TableSetColumnIndex(1);
        if (ImGui::BeginChild("ProjectActionsCard", ImVec2(0.0F, 0.0F), ImGuiChildFlags_Borders)) {
        }
        ImGui::EndChild();

        ImGui::EndTable();
      }
    }

    ImGui::EndChild();
    ImGui::PopStyleVar(3);
  }
  ImGui::End();
}

void ProjectWindowPanel::OnUpdate() {}

void ProjectWindowPanel::DrawRecentProjects()
{
  ImGui::TextUnformatted("Recent Projects");
  ImGui::SameLine();
  ImGui::TextDisabled("(%zu)", m_PreviousProjects_.size());
  ImGui::Separator();

  const float footerHeight = ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y;
  if (ImGui::BeginTable("RecentProjects",
                        2,
                        ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp,
                        ImVec2(0.0F, std::max(80.0F, ImGui::GetContentRegionAvail().y - footerHeight)))) {
    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableSetupColumn("Project", ImGuiTableColumnFlags_WidthStretch, 0.42F);
    ImGui::TableSetupColumn("Location", ImGuiTableColumnFlags_WidthStretch, 0.58F);
    ImGui::TableHeadersRow();

    if (m_PreviousProjects_.empty()) {
      ImGui::TableNextRow(ImGuiTableRowFlags_None, ImGui::GetTextLineHeightWithSpacing() * 3.0F);
      ImGui::TableSetColumnIndex(0);
      ImGui::TextDisabled("No recent projects yet.");
      ImGui::TableSetColumnIndex(1);
      ImGui::TextDisabled("Create one or browse for an existing project.");
    }

    for (std::size_t index = 0; index < m_PreviousProjects_.size(); ++index) {
      const auto& project = m_PreviousProjects_[index];
      ImGui::PushID(static_cast<int>(index));
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);

      const bool isSelected = m_SelectedRecentProject_ == static_cast<int>(index);
      if (ImGui::Selectable(project.c_str(),
                            isSelected,
                            ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick,
                            ImVec2(0.0F, ImGui::GetFrameHeight()))) {
        m_SelectedRecentProject_ = static_cast<int>(index);
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
          OpenProject(project);
        }
      }

      ImGui::TableSetColumnIndex(1);
      ImGui::TextDisabled("%s", project.parent_path().string().c_str());
      if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", project.string().c_str());
      }
      ImGui::PopID();
    }

    ImGui::EndTable();
  }

  const bool hasSelection = m_SelectedRecentProject_ >= 0 && m_SelectedRecentProject_ < static_cast<int>(m_PreviousProjects_.size());
  ImGui::BeginDisabled(!hasSelection);
  if (ImGui::Button("Open Selected Project", ImVec2(-1.0F, 34.0F)) && hasSelection) {
    OpenProject(m_PreviousProjects_[static_cast<std::size_t>(m_SelectedRecentProject_)]);
  }
  ImGui::EndDisabled();
}

void ProjectWindowPanel::OpenProject(const std::filesystem::path& projectPath) {}

} // namespace psych::ui
