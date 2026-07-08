 
/**************************************************************************/
/*  OpenglBuffer.h                                                        */                                                            
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

#pragma once

#include <cstdint>

#include "Renderer/Buffer.h"
#include "Renderer/VertexFormat.h"

namespace psych
{
class OpenglVertexBuffer : public VertexBuffer
{
public:
  OpenglVertexBuffer() = default;
  OpenglVertexBuffer(const float* vertices, uint32_t size);
  OpenglVertexBuffer(uint32_t size);
  virtual ~OpenglVertexBuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;
  virtual void SetData(const float* vertices, const uint32_t size) override;
  virtual void SetLayout(const VertexFormatID ID) override;
  virtual const VertexFormatDesc& GetVertexLayout() const override;

private:
  uint32_t m_RendererID_ = 0;
  VertexFormatDesc m_VertexFormat_;
};

class OpenglIndexBuffer : public IndexBuffer
{
public:
  OpenglIndexBuffer() = default;
  OpenglIndexBuffer(const uint32_t* indices, const uint32_t count);
  virtual ~OpenglIndexBuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;
  virtual uint32_t GetIndexCount() const override;

private:
  uint32_t m_RendererID_ = 0;
  uint32_t m_Count_      = 0;
};
} // namespace psych
