#pragma once

#include <cstdint>

#include "Renderer/Buffer.h"

namespace ge
{
class OpenglVertexBuffer : public VertexBuffer
{
public:
  OpenglVertexBuffer(const float* vertices);
  ~OpenglVertexBuffer() override;

  void Bind() const override;
  void Unbind() const override;

private:
  uint32_t m_RendererID_ = 0;
};

class OpenglIndexBuffer : public IndexBuffer
{
public:
  OpenglIndexBuffer(const float* vertices);
  virtual ~OpenglIndexBuffer();

  void Bind() const override;
  void Unbind() const override;

private:
  uint32_t m_RendererID_ = 0;
};
} // namespace ge
