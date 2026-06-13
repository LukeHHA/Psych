#pragma once

#include "Layers/Layer.h"

namespace ge
{
class EditorLayer : public Layer
{
public:
  EditorLayer();
  virtual ~EditorLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnEvent(Event& event) override;
  virtual void Begin();
  virtual void End();
  virtual void OnUpdate(float ts = 1) override;
  virtual void OnRender() override;
  virtual void SetDarkThemeColors();
  virtual uint32_t GetActiveWidgetID() const;
  virtual void OnImGuiRender() override;

private:
};
} // namespace ge
