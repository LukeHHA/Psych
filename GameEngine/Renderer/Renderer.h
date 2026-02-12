#pragma once

#include "Core/Base.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/RendererContext.h"
#include <glm/vec3.hpp>

namespace ge
{
class Renderer
{
public:
  ~Renderer() = default;

  void SetClearColour(glm::vec3& color);

private:
  Unique<RendererAPI> m_RendererAPI_;
  Unique<RendererContext> m_RendererContext_;
};
} // namespace ge
