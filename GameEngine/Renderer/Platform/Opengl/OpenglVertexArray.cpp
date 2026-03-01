#include "OpenglVertexArray.h"
#include "Core/Base.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"

namespace ge
{
OpenglVertexArray::OpenglVertexArray(Unique<VertexBuffer> vertexBuffer,
                                     Unique<IndexBuffer> indexBuffer)
{
  CORE_ASSERT(vertexBuffer, "vertex buffer is nullptr");
  CORE_ASSERT(indexBuffer, "index buffer is nullptr");

  m_VertexBuffers_.push_back(std::move(vertexBuffer));
  m_IndexBuffer_ = std::move(indexBuffer);

  glGenVertexArrays(1, &m_RendererID_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
}

Unique<VertexArray> OpenglVertexArray::Create(Unique<VertexBuffer> vertexBuffer,
                                              Unique<IndexBuffer> indexBuffer)
{
  return CreateUnique<OpenglVertexArray>(std::move(vertexBuffer),
                                         std::move(indexBuffer));
}
} // namespace ge
