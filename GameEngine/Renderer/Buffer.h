#pragma once

#include "Core/Base.h"
namespace ge
{
class VertexBuffer
{
public:
  virtual ~VertexBuffer();

  virtual void Bind() const   = 0;
  virtual void Unbind() const = 0;

  static Unique<VertexBuffer> Create(const float* vertices);
};

class IndexBuffer
{
public:
  virtual ~IndexBuffer();

  virtual void Bind() const              = 0;
  virtual void Unbind() const            = 0;
  virtual uint32_t GetIndexCount() const = 0;

  static Unique<IndexBuffer> Create(const float* vertices);
};
} // namespace ge
