#pragma once

#include <imgui.h>
#include <string>

namespace psych
{
class ProjectWindow
{
public:
  ProjectWindow()                                = default;
  ProjectWindow(const ProjectWindow&)            = default;
  ProjectWindow(ProjectWindow&&)                 = delete;
  ProjectWindow& operator=(const ProjectWindow&) = default;
  ProjectWindow& operator=(ProjectWindow&&)      = delete;
  virtual ~ProjectWindow()                       = default;

  void OnImGuiRender();

private:
  static inline std::string m_ProjectPath_;
};
} // namespace psych
