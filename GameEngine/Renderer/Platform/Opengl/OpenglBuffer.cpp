#include "OpenglBuffer.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"
#include "GLFW/glfw3.h"

namespace ge
{

// VERTEX BUFFER
OpenglVertexBuffer::OpenglVertexBuffer(const float* vertices, uint32_t count)
{
  CORE_ASSERT(vertices, "Vertices is nullptr");
  glGenBuffers(1, &m_RendererID_);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID_);
  glBufferData(GL_ARRAY_BUFFER, count, vertices, GL_STATIC_DRAW);
}

void OpenglVertexBuffer::Bind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID_);
}
void OpenglVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// INDEX BUFFER

OpenglIndexBuffer::OpenglIndexBuffer(const uint32_t* indices,
                                     const uint32_t count)
    : m_Count_(count)
{
  CORE_ASSERT(indices, "Indices is nullptr");
  glGenBuffers(1, &m_RendererID_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(indices), indices,
               GL_STATIC_DRAW);
}

void OpenglIndexBuffer::Bind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID_);
}
void OpenglIndexBuffer::Unbind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t OpenglIndexBuffer::GetIndexCount() const { return m_Count_; }
} // namespace ge
