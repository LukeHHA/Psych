#pragma once

#include "UI/UILib.h"
#include "imgui.h"
#include <string>

namespace ge
{
class ImGuiUILib : public UILib
{
public:
  ImGuiUILib()          = default;
  virtual ~ImGuiUILib() = default;

  virtual void ToolTip(const std::string& desc) override
  {
    if (ImGui::BeginItemTooltip()) {
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted(desc.c_str());
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }
  }

private:
};
} // namespace ge
