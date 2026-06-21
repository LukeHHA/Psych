#pragma once

#include <cstdint>

#include "Renderer/Buffer.h"
#include "Renderer/VertexFormat.h"

namespace ge
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
} // namespace ge
