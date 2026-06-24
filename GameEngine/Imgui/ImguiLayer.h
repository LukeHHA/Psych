#pragma once

#include "Config/DefaultConfig.h"
#include "Layers/Layer.h"
namespace ge
{

class ImGuiLayer : public Layer
{
public:
  ImGuiLayer();
  ImGuiLayer(const ImGuiLayer&)            = default;
  ImGuiLayer(ImGuiLayer&&)                 = delete;
  ImGuiLayer& operator=(const ImGuiLayer&) = delete;
  ImGuiLayer& operator=(ImGuiLayer&&)      = delete;
  ~ImGuiLayer() override                   = default;

  void OnAttach() override;
  void OnDetach() override;
  void OnEvent(Event& e) override;
  void OnUpdate(float ts = 1) override {}
  void OnRender() override {}

  static void Begin();
  static void End();

  void OnImGuiRender() override;
  // void BlockEvents(bool block) { m_BlockEvents = block; }
  // void SetDarkThemeColors();
  // uint32_t GetActiveWidgetID() const;

private:
  void ConfigureEditorUIRuntime();
  void ApplyEditorTheme();
  void LoadEditorFont();

private:
  bool m_BlockEvents = true;
  EditorUISpec m_EditorSpec_;
};

} // namespace ge
