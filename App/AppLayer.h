#pragma once

#include "Layers/Layer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

class AppLayer : public psych::Layer
{
public:
  AppLayer();
  virtual ~AppLayer();

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnEvent(psych::Event& event) override;
  virtual void OnUpdate(float ts = 1) override;
  virtual void OnRender() override;
  virtual void OnImGuiRender() override;

private:
  psych::Shared<psych::VertexArray> m_VertexArray;
  psych::Shared<psych::Shader> m_Shader;
};
