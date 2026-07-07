#pragma once

#include "Core/Core.h"
#include "Layers/Layer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include <cstdint>

namespace psych
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
  virtual void OnImGuiRender() override;

private:
  Shared<VertexArray> m_CubeVertexArray_;
  Shared<Shader> m_CubeShader_;
  Unique<RendererAPI> m_RendererAPI_;
  float m_CubeRotation_      = 0.0f;
  uint32_t m_ViewportWidth_  = 1280;
  uint32_t m_ViewportHeight_ = 720;
};
} // namespace psych
