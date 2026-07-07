#pragma once

#include <cstdint>
#include <vector>

#include "../../VertexArray.h"
#include "Core/Core.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexFormat.h"

namespace psych
{
class OpenglVertexArray : public VertexArray
{
public:
  OpenglVertexArray();
  OpenglVertexArray(const OpenglVertexArray&)            = default;
  OpenglVertexArray(OpenglVertexArray&&)                 = delete;
  OpenglVertexArray& operator=(const OpenglVertexArray&) = default;
  OpenglVertexArray& operator=(OpenglVertexArray&&)      = delete;
  ~OpenglVertexArray() override;

  void Bind() const override;
  void Unbind() const override;
  void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer) override;
  void AddIndexBuffer(const Shared<IndexBuffer>& indexBuffer) override;
  [[nodiscard]] const Shared<VertexBuffer>& GetVertexBuffer(const std::size_t index) const override { return m_VertexBuffers_.at(index); }
  [[nodiscard]] const Shared<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer_; };
  static Shared<VertexArray> Create();

  static GLenum ToOpenGLBaseType(ShaderDataType type)
  {
    switch (type) {
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

  static GLint ComponentCount(ShaderDataType type)
  {
    switch (type) {
    case ShaderDataType::Float2:
      return 2;
    case ShaderDataType::Float3:
      return 3;
    default:
      CORE_ASSERT(false, "Unknown ShaderDataType");
      return 0;
    }
  }

  static bool IsIntegerType(ShaderDataType type)
  {
    switch (type) {
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
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
} // namespace psych
