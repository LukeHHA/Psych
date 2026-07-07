#pragma once

#include "Renderer/VertexFormat.h"

namespace psych
{
class Mesh
{
public:
  Mesh()                                            = default;
  virtual ~Mesh()                                   = default;

  virtual bool IsDirty() const                      = 0;
  virtual const VertexFormatDesc& GetLayout() const = 0;
};
} // namespace psych
