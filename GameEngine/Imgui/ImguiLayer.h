#pragma once

#include "Layers/Layer.h"
namespace ge
{

class ImGuiLayer : public Layer
{
public:
  ImGuiLayer();
  ~ImGuiLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnEvent(Event& e) override;
  virtual void OnUpdate(float ts = 1) override {}
  virtual void OnRender() override {}

  static void Begin();
  static void End();

  void OnImGuiRender() override;
  // void BlockEvents(bool block) { m_BlockEvents = block; }
  // void SetDarkThemeColors();
  // uint32_t GetActiveWidgetID() const;

private:
  bool m_BlockEvents = true;
};

} // namespace ge
