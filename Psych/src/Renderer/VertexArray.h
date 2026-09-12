 
/**************************************************************************/
/*  VertexArray.h                                                         */                                                            
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

#include "Renderer/Buffer.h"
namespace psych
{
class VertexArray
{
public:
  VertexArray()                                                                              = default;
  VertexArray(const VertexArray&)                                                            = default;
  VertexArray(VertexArray&&)                                                                 = delete;
  VertexArray& operator=(const VertexArray&)                                                 = default;
  VertexArray& operator=(VertexArray&&)                                                      = delete;
  virtual ~VertexArray()                                                                     = default;

  virtual void Bind() const                                                                  = 0;
  virtual void Unbind() const                                                                = 0;

  [[nodiscard]] virtual const Shared<VertexBuffer>& GetVertexBuffer(std::size_t index) const = 0;
  [[nodiscard]] virtual const Shared<IndexBuffer>& GetIndexBuffer() const                    = 0;

  virtual void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer)                     = 0;
  virtual void AddIndexBuffer(const Shared<IndexBuffer>& indexBuffer)                        = 0;

  static Shared<VertexArray> Create();
};
} // namespace psych
