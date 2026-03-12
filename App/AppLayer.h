#pragma once

#include "ge/Core/Layer.h"
#include "ge/Core/Renderer.h"

class AppLayer : public ge::Layer
{
public:
  AppLayer();
  virtual ~AppLayer();

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnEvent(ge::Event& event) override;
  virtual void OnUpdate(float ts = 1) override;
  virtual void OnRender() override;
  virtual void OnImGuiRender() override;

private:
  ge::Shared<ge::VertexArray> m_VertexArray;
  ge::Shared<ge::Shader> m_Shader;
};
