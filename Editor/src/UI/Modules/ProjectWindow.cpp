#include "ProjectWindow.h"
#include "Core/PsychEngine.h"
#include "FileSystem/FileSystem.h"
#include <algorithm>
#include <filesystem>
#include <imgui.h>
#include <utility>

namespace psych::ui
{
ProjectWindowPanel::ProjectWindowPanel()
{
  const auto& projectSpec = PsychEngine::Get().GetEngineSpecification().Projects;
  const std::filesystem::path previousProjectsDirectory{projectSpec.PreviousProjectsDir};

  m_PreviousProjects_.reserve(projectSpec.PreviousProjects.size());
  for (const auto& previousProject : projectSpec.PreviousProjects) {
    std::filesystem::path projectPath{previousProject};
    if (projectPath.is_relative() && !previousProjectsDirectory.empty()) {
      projectPath = previousProjectsDirectory / projectPath;
    }

    m_PreviousProjects_.push_back(projectPath.lexically_normal());
  }
}

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
          RecentProjects();
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::TableSetColumnIndex(1);
        if (ImGui::BeginChild("ProjectActionsCard", ImVec2(0.0F, 0.0F), ImGuiChildFlags_Borders)) {
          ImGui::TextUnformatted("New Project");
          ImGui::Separator();
          ImGui::Dummy(ImVec2(0.0F, 6.0F));
          ImGui::TextWrapped("Choose a folder for the project. Psych will use that folder as the project root and open it in the editor.");
          ImGui::Dummy(ImVec2(0.0F, 14.0F));

          if (ImGui::Button("New Project", ImVec2(-1.0F, 38.0F))) {
            const std::filesystem::path projectPath = Filesystem::GetFileExplorer();
            if (!projectPath.empty()) {
              OpenProject(projectPath);
            }
          }

          if (!m_StatusMessage_.empty()) {
            ImGui::Dummy(ImVec2(0.0F, 8.0F));
            if (m_StatusIsError_) {
              ImGui::TextColored(ImVec4(0.95F, 0.35F, 0.35F, 1.0F), "%s", m_StatusMessage_.c_str());
            } else {
              ImGui::TextWrapped("%s", m_StatusMessage_.c_str());
            }
          }
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

void ProjectWindowPanel::RecentProjects()
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
      const auto& project       = m_PreviousProjects_[index];
      std::string projectName   = project.filename().string();
      std::string projectParent = project.parent_path().string();
      if (projectName.empty()) {
        projectName   = project.parent_path().filename().string();
        projectParent = project.parent_path().parent_path().string();
      }

      ImGui::PushID(static_cast<int>(index));
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);

      const bool isSelected = m_SelectedRecentProject_ == static_cast<int>(index);
      if (ImGui::Selectable(projectName.c_str(),
                            isSelected,
                            ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick,
                            ImVec2(0.0F, ImGui::GetFrameHeight()))) {
        m_SelectedRecentProject_ = static_cast<int>(index);
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
          OpenProject(project);
        }
      }

      ImGui::TableSetColumnIndex(1);
      ImGui::TextDisabled("%s", projectParent.c_str());
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

void ProjectWindowPanel::OpenProject(const std::filesystem::path& projectPath)
{
  if (!m_ProjectSelectedCallback_) {
    m_StatusMessage_ = "The editor is not ready to open a project.";
    m_StatusIsError_ = true;
    return;
  }

  if (!m_ProjectSelectedCallback_(projectPath)) {
    m_StatusMessage_ = "Unable to open the selected project folder.";
    m_StatusIsError_ = true;
    return;
  }

  m_StatusMessage_.clear();
  m_StatusIsError_ = false;
}

void ProjectWindowPanel::SetProjectSelectedCallbackFn(ProjectSelectedCallbackFn callback) { m_ProjectSelectedCallback_ = std::move(callback); }

} // namespace psych::ui
