#include "AppLayer.h"
#include "Core/Core.h"
#include "Debug/Instrumentor.h"
#include "Renderer/Buffer.h"
#include "Renderer/RendererAPI.h"

AppLayer::AppLayer() : psych::Layer("AppLayer") { CORE_PROFILE_FUNCTION(); }

AppLayer::~AppLayer() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnUpdate(float ts)
{
  CORE_PROFILE_FUNCTION();

  auto renderer = psych::RendererAPI::Create();
  if (!renderer) {
    return;
  }
  renderer.value()->DrawIndexed(m_VertexArray, 6);
}
void AppLayer::OnEvent(psych::Event& event) { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnRender() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnAttach()
{
  CORE_PROFILE_FUNCTION();
  auto shader = psych::Shader::Create("data/Shaders/vertex.glsl", "data/Shaders/fragment.glsl", "TestShader");
  if (!shader) {
    return;
  }
  m_Shader = std::move(shader.value());
  m_Shader->Bind();
  static const float s_TriangleVertices[] = {
      // pos xyz         // color rgb
      0.5f,  0.5f,  0.0f, 0.5f, 0.6f, 0.1f, // 0 top right
      0.5f,  -0.5f, 0.0f, 0.3f, 0.6f, 0.3f, // 1 bottom right
      -0.5f, -0.5f, 0.0f, 0.9f, 0.5f, 0.2f, // 2 bottom left
      -0.5f, 0.5f,  0.0f, 0.4f, 0.6f, 0.8f  // 3 top left
  };

  static const uint32_t s_TriangleIndices[] = {
      0,
      1,
      3, // first triangle
      1,
      2,
      3  // second triangle
  };

  auto vertexBuffer = psych::VertexBuffer::Create(s_TriangleVertices, sizeof(s_TriangleVertices));
  vertexBuffer->SetLayout(psych::VertexFormatID::PC);
  auto IndexBuffer = psych::IndexBuffer::Create(s_TriangleIndices, 6);
  m_VertexArray    = psych::VertexArray::Create();
  m_VertexArray->AddVertexBuffer(vertexBuffer);
  m_VertexArray->AddIndexBuffer(IndexBuffer);
}

void AppLayer::OnDetach() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnImGuiRender() {}
