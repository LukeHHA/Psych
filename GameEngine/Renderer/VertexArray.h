#pragma once

#include "Renderer/Buffer.h"
namespace ge
{
class VertexArray
{
public:
  virtual ~VertexArray();

  virtual void Bind() const                                              = 0;
  virtual void Unbind() const                                            = 0;

  virtual Shared<VertexBuffer>& GetVertexBuffer() const                  = 0;
  virtual const Shared<IndexBuffer>& GetIndexBuffer() const              = 0;

  virtual void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer) = 0;
  virtual void AddIndexBuffer(const Shared<IndexBuffer>& indexBuffer)    = 0;
};
} // namespace ge
