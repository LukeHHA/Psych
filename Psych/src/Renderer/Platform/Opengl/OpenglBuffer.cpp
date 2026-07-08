 
/**************************************************************************/
/*  OpenglBuffer.cpp                                                      */                                                            
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

#include "OpenglBuffer.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"
#include "Renderer/VertexFormat.h"

namespace psych
{

// VERTEX BUFFER
OpenglVertexBuffer::OpenglVertexBuffer(const float* vertices, uint32_t size)
{
  CORE_ASSERT(vertices, "Vertices is nullptr");
  glGenBuffers(1, &m_RendererID_);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID_);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenglVertexBuffer::OpenglVertexBuffer(uint32_t size)
{
  glGenBuffers(1, &m_RendererID_);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID_);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
}

OpenglVertexBuffer::~OpenglVertexBuffer() { glDeleteBuffers(1, &m_RendererID_); }

void OpenglVertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID_); }

void OpenglVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void OpenglVertexBuffer::SetLayout(const VertexFormatID ID) { m_VertexFormat_ = GetVertexFormat(ID); }

void OpenglVertexBuffer::SetData(const float* vertices, const uint32_t size)
{
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}

const VertexFormatDesc& OpenglVertexBuffer::GetVertexLayout() const { return m_VertexFormat_; }

// INDEX BUFFER

OpenglIndexBuffer::OpenglIndexBuffer(const uint32_t* indices, const uint32_t count) : m_Count_(count)
{
  CORE_ASSERT(indices, "Indices is nullptr");
  glGenBuffers(1, &m_RendererID_);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID_);
  glBufferData(GL_ARRAY_BUFFER, count * sizeof(indices), indices, GL_STATIC_DRAW);
}

OpenglIndexBuffer::~OpenglIndexBuffer() { glDeleteBuffers(1, &m_RendererID_); }

void OpenglIndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID_); }
void OpenglIndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

uint32_t OpenglIndexBuffer::GetIndexCount() const { return m_Count_; }
} // namespace psych
