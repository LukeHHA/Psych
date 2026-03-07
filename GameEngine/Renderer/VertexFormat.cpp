#include "VertexFormat.h"
#include "VertexTypes.h"

namespace ge
{

const VertexFormatDesc& GetVertexFormat(VertexFormatID id)
{

  static const std::array<VertexAttributeDesc, 2> PC_Attrs = {{
      {ShaderDataType::Float3, 0, offsetof(VertexPC, POSITION), false},
      {ShaderDataType::Float3, 1, offsetof(VertexPC, COLOR), false},
  }};
  static const VertexFormatDesc PC = {sizeof(VertexPC), PC_Attrs};

  static const std::array<VertexAttributeDesc, 3> PNC_Attrs = {{
      {ShaderDataType::Float3, 0, offsetof(VertexPNC, POSITION), false},
      {ShaderDataType::Float3, 1, offsetof(VertexPNC, NORMAL), false},
      {ShaderDataType::Float3, 2, offsetof(VertexPNC, COLOR), false},
  }};
  static const VertexFormatDesc PNC = {sizeof(VertexPNC), PNC_Attrs};

  switch (id) {
  case VertexFormatID::PC:
    return PC;
  case VertexFormatID::PNC:
    return PNC;
  default:
    return PNC;
  }
}
} // namespace ge
