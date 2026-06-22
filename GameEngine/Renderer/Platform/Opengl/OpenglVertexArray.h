#pragma once

#include <cstdint>
#include <vector>

#include "../../VertexArray.h"
#include "Core/Core.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexFormat.h"

namespace ge
{
class OpenglVertexArray : public VertexArray
{
public:
  OpenglVertexArray();
  ~OpenglVertexArray() override;

  virtual void Bind() const override;
  virtual void Unbind() const override;
  virtual void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer) override;
  virtual void AddIndexBuffer(const Shared<IndexBuffer>& indexBuffer) override;
  virtual const Shared<VertexBuffer>& GetVertexBuffer(const std::size_t index) const override { return m_VertexBuffers_.at(index); }
  virtual const Shared<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer_; };
  static Shared<VertexArray> Create();

  inline static GLenum ToOpenGLBaseType(ShaderDataType t)
  {
    switch (t) {
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
      return GL_FLOAT;
    case ShaderDataType::Int3:
      return GL_INT;
    default:
      CORE_ASSERT(false, "Unknown ShaderDataType");
      return 0;
    }
  }

  inline static GLint ComponentCount(ShaderDataType t)
  {
    switch (t) {
    case ShaderDataType::Float2:
      return 2;
    case ShaderDataType::Float3:
      return 3;
    default:
      CORE_ASSERT(false, "Unknown ShaderDataType");
      return 0;
    }
  }

  inline static bool IsIntegerType(ShaderDataType t)
  {
    switch (t) {
    case ShaderDataType::Int2:
    case ge::ShaderDataType::Int3:
      return true;
    default:
      return false;
    }
  }

private:
  uint32_t m_RendererID_ = 0;
  std::vector<Shared<VertexBuffer>> m_VertexBuffers_;
  Shared<IndexBuffer> m_IndexBuffer_;
};
} // namespace ge
