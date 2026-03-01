#include "OpenglBuffer.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"
#include "GLFW/glfw3.h"

namespace ge
{

// VERTEX BUFFER
OpenglVertexBuffer::OpenglVertexBuffer(const float* vertices)
{
  CORE_ASSERT(vertices, "Vertices is nullptr");
  glGenBuffers(1, &m_RendererID_);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void OpenglVertexBuffer::Bind() const { glBindBuffer(1, m_RendererID_); }
void OpenglVertexBuffer::Unbind() const { glBindBuffer(0, m_RendererID_); }

// INDEX BUFFER

OpenglIndexBuffer::OpenglIndexBuffer(const float* indices, const uint32_t count)
    : m_Count_(count)
{
  CORE_ASSERT(indices, "Indices is nullptr");
  glGenBuffers(1, &m_RendererID_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
}

void OpenglIndexBuffer::Bind() const { glBindBuffer(1, m_RendererID_); }
void OpenglIndexBuffer::Unbind() const { glBindBuffer(0, m_RendererID_); }

uint32_t OpenglIndexBuffer::GetIndexCount() const { return m_Count_; }
} // namespace ge
