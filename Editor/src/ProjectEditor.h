#pragma once

#include "Core/Core.h"
#include "Layers/Layer.h"
#include "Renderer/RendererAPI.h"

namespace psych
{
class ProjectEditor : public Layer
{
public:
  ProjectEditor();
  ~ProjectEditor() override = default;

  void OnAttach() override;
  void OnDetach() override;
  void OnEvent(Event& event) override;
  virtual void Begin();
  virtual void End();
  void OnUpdate(float ts = 1) override;
  void OnRender() override;
  void OnImGuiRender() override;

private:
  Unique<RendererAPI> m_RendererAPI_;
};
} // namespace psych
