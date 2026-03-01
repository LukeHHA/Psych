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
  OpenglVertexArray(Unique<VertexBuffer> vertexBuffer,
                    Unique<IndexBuffer> indexBuffer);
  ~OpenglVertexArray() override;

  void Bind() const override;
  void Unbind() const override;

  static Unique<VertexArray> Create(Unique<VertexBuffer> vertexBuffer,
                                    Unique<IndexBuffer> indexBuffer);

private:
  uint32_t m_RendererID_ = 0;
  std::vector<Unique<VertexBuffer>> m_VertexBuffers_;
  Unique<IndexBuffer> m_IndexBuffer_;
};
} // namespace ge
