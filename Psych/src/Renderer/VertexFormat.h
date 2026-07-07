#pragma once

#include <cstdint>
#include <span>

namespace psych
{

enum class VertexFormatID {
  PC,
  PNC,
};
enum class ShaderDataType {
  Float2,
  Float3,
  Int2,
  Int3,
};

struct VertexAttributeDesc {
  ShaderDataType type;
  uint32_t location;
  uint32_t offset;
  bool normalized;
};

struct VertexFormatDesc {
  uint32_t stride;
  std::span<const VertexAttributeDesc> attrs;
};

const VertexFormatDesc& GetVertexFormat(VertexFormatID id);
} // namespace psych
