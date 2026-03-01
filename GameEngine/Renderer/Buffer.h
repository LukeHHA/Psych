#pragma once

#include "Core/Base.h"
namespace ge
{
class VertexBuffer
{
public:
  VertexBuffer()              = default;
  virtual ~VertexBuffer()     = default;

  virtual void Bind() const   = 0;
  virtual void Unbind() const = 0;

  static Shared<VertexBuffer> Create(const float* vertices, uint32_t count);
};

class IndexBuffer
{
public:
  IndexBuffer()                          = default;
  virtual ~IndexBuffer()                 = default;

  virtual void Bind() const              = 0;
  virtual void Unbind() const            = 0;
  virtual uint32_t GetIndexCount() const = 0;

  static Shared<IndexBuffer> Create(const uint32_t* indices,
                                    const uint32_t count);
};
} // namespace ge
