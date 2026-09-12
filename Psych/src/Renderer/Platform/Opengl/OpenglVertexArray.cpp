 
/**************************************************************************/
/*  OpenglVertexArray.cpp                                                 */                                                            
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PSYCH ENGINE                               */
/**************************************************************************/
/* Copyright (c)  Luke Howe                                               */                                                  
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "OpenglVertexArray.h"
#include "Core/Core.h"
#include "Debug/Assert.h"

namespace psych
{
OpenglVertexArray::OpenglVertexArray() { glGenVertexArrays(1, &m_RendererID_); }

OpenglVertexArray::~OpenglVertexArray() { glDeleteVertexArrays(1, &m_RendererID_); }

void OpenglVertexArray::Bind() const { glBindVertexArray(m_RendererID_); }

void OpenglVertexArray::Unbind() const { glBindVertexArray(0); }

void OpenglVertexArray::AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer)
{
  CORE_ASSERT(vertexBuffer, "vertex buffer is nullptr");
  glBindVertexArray(m_RendererID_);
  vertexBuffer->Bind();
  auto& fmt = vertexBuffer->GetVertexLayout();

  // set attributes
  CORE_ASSERT(!fmt.attrs.empty(),
              "Vertex format has no attributes, You may "
              "have forgotten to set the layout");
  CORE_ASSERT(fmt.stride > 0, "Vertex format stride is 0");

  for (const auto& a : fmt.attrs) {
    const GLuint loc = a.location;
    glEnableVertexAttribArray(loc);

    const GLenum glType  = ToOpenGLBaseType(a.type);
    const GLint count    = ComponentCount(a.type);
    const GLboolean norm = a.normalized ? GL_TRUE : GL_FALSE;
    const GLsizei stride = static_cast<GLsizei>(fmt.stride);
    const void* ptr      = reinterpret_cast<const void*>(static_cast<uintptr_t>(a.offset));

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

Shared<VertexArray> OpenglVertexArray::Create() { return CreateShared<OpenglVertexArray>(); }
} // namespace psych
