#include "AppLayer.h"
#include "Debug/Instrumentor.h"
#include "Renderer/Buffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexFormat.h"

AppLayer::AppLayer() : ge::Layer("AppLayer") { CORE_PROFILE_FUNCTION(); }

AppLayer::~AppLayer() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnUpdate(float ts)
{
  CORE_PROFILE_FUNCTION();

  auto renderer = ge::RendererAPI::Create();
  renderer->DrawIndexed(m_VertexArray, 6);
}
void AppLayer::OnEvent(ge::Event& event) { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnRender() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnAttach()
{
  CORE_PROFILE_FUNCTION();
  const auto shader = ge::Shader::Create(
      "out/Shaders/vertex.glsl", "out/Shaders/fragment.glsl", "TestShader");

  shader->Bind();
  static const float s_TriangleVertices[] = {
      // pos xyz         // color rgb
      0.5f,  0.5f,  0.0f, 0.5f, 0.6f, 0.1f, // 0 top right
      0.5f,  -0.5f, 0.0f, 0.3f, 0.6f, 0.3f, // 1 bottom right
      -0.5f, -0.5f, 0.0f, 0.9f, 0.5f, 0.2f, // 2 bottom left
      -0.5f, 0.5f,  0.0f, 0.4f, 0.6f, 0.8f  // 3 top left
  };

  static const uint32_t s_TriangleIndices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  auto vertexBuffer =
      ge::VertexBuffer::Create(s_TriangleVertices, sizeof(s_TriangleVertices));
  vertexBuffer->SetLayout(ge::VertexFormatID::PC);
  auto IndexBuffer = ge::IndexBuffer::Create(s_TriangleIndices, 6);
  m_VertexArray    = ge::VertexArray::Create();
  m_VertexArray->AddVertexBuffer(vertexBuffer);
  m_VertexArray->AddIndexBuffer(IndexBuffer);
}

void AppLayer::OnDetach() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnImGuiRender() {}
