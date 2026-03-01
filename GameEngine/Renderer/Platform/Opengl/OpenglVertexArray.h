#pragma once

#include <cstdint>
#include <vector>

#include "../../VertexArray.h"
#include "Core/Base.h"
#include "Renderer/Buffer.h"

namespace ge
{
class OpenglVertexArray : public VertexArray
{
public:
  OpenglVertexArray();
  ~OpenglVertexArray() override;

  virtual void Bind() const override;
  virtual void Unbind() const override;
  virtual void
  AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer) override;
  virtual void AddIndexBuffer(const Shared<IndexBuffer>& indexBuffer) override;
  virtual Shared<VertexBuffer>& GetVertexBuffer() const override;
  virtual const Shared<IndexBuffer>& GetIndexBuffer() const override
  {
    return m_IndexBuffer_;
  };
  static Unique<VertexArray> Create();

private:
  uint32_t m_RendererID_ = 0;
  std::vector<Shared<VertexBuffer>> m_VertexBuffers_;
  Shared<IndexBuffer> m_IndexBuffer_;
};
} // namespace ge
