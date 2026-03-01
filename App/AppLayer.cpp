#include "AppLayer.h"
#include "Debug/Instrumentor.h"
#include "Renderer/Buffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Shader.h"

AppLayer::AppLayer() : ge::Layer("AppLayer") { CORE_PROFILE_FUNCTION(); }

AppLayer::~AppLayer() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnUpdate(float ts)
{
  CORE_PROFILE_FUNCTION();

  auto renderer = ge::RendererAPI::Create();
  renderer->DrawIndexed(m_VertexArray, 3);
}
void AppLayer::OnEvent(ge::Event& event) { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnRender() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnAttach()
{
  CORE_PROFILE_FUNCTION();
  const auto shader = ge::Shader::Create(
      "out/Shaders/vertex.vs", "out/Shaders/fragment.fs", "TestShader");

  shader->Bind();
  static const float s_TriangleVertices[] = {
      // x,     y,    z,     r,   g,   b
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left  (red)
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-right (green)
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top          (blue)
  };

  static const uint32_t s_TriangleIndices[] = {0, 1, 2};

  auto vertexBuffer =
      ge::VertexBuffer::Create(s_TriangleVertices, sizeof(s_TriangleVertices));
  auto IndexBuffer = ge::IndexBuffer::Create(s_TriangleIndices, 3);
  m_VertexArray    = ge::VertexArray::Create();
  m_VertexArray->AddVertexBuffer(vertexBuffer);
  m_VertexArray->AddIndexBuffer(IndexBuffer);
}

void AppLayer::OnDetach() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnImGuiRender() {}
