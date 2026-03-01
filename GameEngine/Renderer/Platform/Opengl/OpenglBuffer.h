#pragma once

#include <cstdint>

#include "Renderer/Buffer.h"

namespace ge
{
class OpenglVertexBuffer : public VertexBuffer
{
public:
  OpenglVertexBuffer(const float* vertices, uint32_t count);
  virtual ~OpenglVertexBuffer() = default;

  virtual void Bind() const override;
  virtual void Unbind() const override;

private:
  uint32_t m_RendererID_ = 0;
};

class OpenglIndexBuffer : public IndexBuffer
{
public:
  OpenglIndexBuffer(const uint32_t* indices, const uint32_t count);
  virtual ~OpenglIndexBuffer() = default;

  virtual void Bind() const override;
  virtual void Unbind() const override;
  virtual uint32_t GetIndexCount() const override;

private:
  uint32_t m_RendererID_ = 0;
  uint32_t m_Count_      = 0;
};
} // namespace ge
