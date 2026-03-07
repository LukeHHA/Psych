#include "OpenglVertexArray.h"
#include "Core/Base.h"
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
  auto& fmt = vertexBuffer->GetVertexLayout();

  // set attributes
  CORE_ASSERT(!fmt.attrs.empty(), "Vertex format has no attributes, You may "
                                  "have forgotten to set the layout");
  CORE_ASSERT(fmt.stride > 0, "Vertex format stride is 0");

  for (const auto& a : fmt.attrs) {
    const GLuint loc = a.location;
    glEnableVertexAttribArray(loc);

    const GLenum glType  = ToOpenGLBaseType(a.type);
    const GLint count    = ComponentCount(a.type);
    const GLboolean norm = a.normalized ? GL_TRUE : GL_FALSE;
    const GLsizei stride = static_cast<GLsizei>(fmt.stride);
    const void* ptr =
        reinterpret_cast<const void*>(static_cast<uintptr_t>(a.offset));

    if (IsIntegerType(a.type)) {
      glVertexAttribIPointer(loc, count, glType, stride, ptr);
    } else {
      glVertexAttribPointer(loc, count, glType, norm, stride, ptr);
    }
  }
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
