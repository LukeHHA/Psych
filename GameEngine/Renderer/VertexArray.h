#pragma once

#include "Renderer/Buffer.h"
namespace ge
{
class VertexArray
{
public:
  VertexArray()                                               = default;
  virtual ~VertexArray()                                      = default;

  virtual void Bind() const                                   = 0;
  virtual void Unbind() const                                 = 0;

  virtual const Shared<VertexBuffer>& GetVertexBuffer() const = 0;
  virtual const Shared<IndexBuffer>& GetIndexBuffer() const   = 0;

  virtual void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer) = 0;
  virtual void AddIndexBuffer(const Shared<IndexBuffer>& indexBuffer)    = 0;

  static Shared<VertexArray> Create();
};
} // namespace ge
