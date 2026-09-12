 
/**************************************************************************/
/*  Buffer.h                                                              */                                                            
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

#include "Core/Core.h"
#include "Renderer/VertexFormat.h"
namespace psych
{
class VertexBuffer
{
public:
  VertexBuffer()                                                   = default;
  virtual ~VertexBuffer()                                          = default;

  virtual void Bind() const                                        = 0;
  virtual void Unbind() const                                      = 0;
  virtual void SetData(const float* vertices, const uint32_t size) = 0;
  virtual void SetLayout(const VertexFormatID ID)                  = 0;
  virtual const VertexFormatDesc& GetVertexLayout() const          = 0;

  static Shared<VertexBuffer> Create(const float* vertices, uint32_t count);
  static Shared<VertexBuffer> Create(uint32_t size);
};

class IndexBuffer
{
public:
  IndexBuffer()                          = default;
  virtual ~IndexBuffer()                 = default;

  virtual void Bind() const              = 0;
  virtual void Unbind() const            = 0;
  virtual uint32_t GetIndexCount() const = 0;

  static Shared<IndexBuffer> Create(const uint32_t* indices, const uint32_t count);
};
} // namespace psych
