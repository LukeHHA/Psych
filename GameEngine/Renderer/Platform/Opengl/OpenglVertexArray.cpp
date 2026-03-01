#include "OpenglVertexArray.h"
#include "Core/Base.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"

namespace ge
{
OpenglVertexArray::OpenglVertexArray() { glGenVertexArrays(1, &m_RendererID_); }

OpenglVertexArray::~OpenglVertexArray()
{
  glDeleteVertexArrays(1, &m_RendererID_);
}

void OpenglVertexArray::Bind() const { glBindVertexArray(m_RendererID_); }

void OpenglVertexArray::Unbind() const { glBindVertexArray(0); }

void OpenglVertexArray::AddVertexBuffer(
    const Shared<VertexBuffer>& vertexBuffer)
{
  CORE_ASSERT(vertexBuffer, "vertex buffer is nullptr");
  glBindVertexArray(m_RendererID_);
  vertexBuffer->Bind();

  // set attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));

  m_VertexBuffers_.push_back(vertexBuffer);
}

void OpenglVertexArray::AddIndexBuffer(const Shared<IndexBuffer>& indexBuffer)
{
  CORE_ASSERT(indexBuffer, "index buffer is nullptr");
  glBindVertexArray(m_RendererID_);
  indexBuffer->Bind();
  m_IndexBuffer_ = indexBuffer;
}

Shared<VertexArray> OpenglVertexArray::Create()
{
  return CreateShared<OpenglVertexArray>();
}
} // namespace ge
