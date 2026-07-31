#pragma once

#include "Core/Core.h"
#include "Panel.h"
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

namespace psych::ui
{
class ProjectWindowPanel : public Panel
{
public:
  using ProjectSelectedCallbackFn = std::function<bool(const std::filesystem::path&)>;

  ProjectWindowPanel();
  ProjectWindowPanel(const ProjectWindowPanel&)            = default;
  ProjectWindowPanel(ProjectWindowPanel&&)                 = delete;
  ProjectWindowPanel& operator=(const ProjectWindowPanel&) = default;
  ProjectWindowPanel& operator=(ProjectWindowPanel&&)      = delete;
  ~ProjectWindowPanel() override                           = default;

  [[nodiscard]] static Unique<ProjectWindowPanel> Create();
  [[nodiscard]] static std::string GetID();

  void OnImGuiRender() override;
  void OnUpdate() override;
  void SetProjectSelectedCallbackFn(ProjectSelectedCallbackFn callback);

private:
  void DrawRecentProjects();
  void OpenProject(const std::filesystem::path& projectPath);

private:
  ProjectSelectedCallbackFn m_ProjectSelectedCallback_;
  std::vector<std::filesystem::path> m_PreviousProjects_;
  std::string m_NewProjectName_ = "New Project";
  std::string m_CreateLocation_;
  std::string m_ExistingProjectPath_;
  std::string m_StatusMessage_;
  int m_SelectedRecentProject_ = -1;
  bool m_StatusIsError_        = false;
};
} // namespace psych::ui
