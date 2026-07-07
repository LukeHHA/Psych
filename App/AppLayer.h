#pragma once

#include "Psych/Core/Layer.h"
#include "Psych/Core/Renderer.h"

class AppLayer : public psychLayer
{
public:
  AppLayer();
  virtual ~AppLayer();

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnEvent(psychEvent& event) override;
  virtual void OnUpdate(float ts = 1) override;
  virtual void OnRender() override;
  virtual void OnImGuiRender() override;

private:
  psychShared<psychVertexArray> m_VertexArray;
  psychShared<psychShader> m_Shader;
};
