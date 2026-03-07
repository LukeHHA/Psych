#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace ge
{

struct VertexPC {
  glm::vec3 POSITION;
  glm::vec3 COLOR;
};

struct VertexPNC {
  glm::vec3 POSITION;
  glm::vec3 NORMAL;
  glm::vec3 COLOR;
};

struct VertexPNT {
  glm::vec3 POSITION;
  glm::vec3 NORMAL;
  glm::vec2 TEXCOORD;
};

struct VertexPNTBT {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec3 Tangent;
  glm::vec3 Binormal;
  glm::vec2 Texcoord;
};

struct Index {
  uint32_t V1, V2, V3;
};

} // namespace ge
