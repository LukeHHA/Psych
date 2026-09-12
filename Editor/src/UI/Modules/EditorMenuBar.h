#pragma once

#include "Core/Core.h"
#include "Panel.h"
#include <string>

namespace psych::ui
{
class EditorMenuBarPanel : public Panel
{
public:
  EditorMenuBarPanel()                                     = default;
  EditorMenuBarPanel(const EditorMenuBarPanel&)            = default;
  EditorMenuBarPanel(EditorMenuBarPanel&&)                 = delete;
  EditorMenuBarPanel& operator=(const EditorMenuBarPanel&) = default;
  EditorMenuBarPanel& operator=(EditorMenuBarPanel&&)      = delete;
  ~EditorMenuBarPanel() override                           = default;

  [[nodiscard]] static Unique<EditorMenuBarPanel> Create();
  [[nodiscard]] static std::string GetID();

  void OnImGuiRender() override;
  void OnUpdate() override;

private:
  void ShowExampleMenuFile();
  void MainMenuBar();
  void WindowMenuBar();

  bool m_OptionsEnabled_ = true;
  bool m_OptionChecked_  = true;
  float m_OptionValue_   = 0.5f;
  int m_OptionSelection_ = 0;
};
} // namespace psych::ui
