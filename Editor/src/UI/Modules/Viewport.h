#pragma once

#include "Core/Core.h"
#include "Panel.h"
#include <string>

namespace psych::ui
{
class ViewportPanel : public Panel
{
public:
  ViewportPanel()                                = default;
  ViewportPanel(const ViewportPanel&)            = default;
  ViewportPanel(ViewportPanel&&)                 = delete;
  ViewportPanel& operator=(const ViewportPanel&) = default;
  ViewportPanel& operator=(ViewportPanel&&)      = delete;
  ~ViewportPanel() override                      = default;

  [[nodiscard]] static Unique<ViewportPanel> Create();
  [[nodiscard]] static std::string GetID();

  void OnImGuiRender() override;
  void OnUpdate() override;
};
} // namespace psych::ui
