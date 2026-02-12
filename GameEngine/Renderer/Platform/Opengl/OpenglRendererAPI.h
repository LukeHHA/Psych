#pragma once

#include "Renderer/RendererAPI.h"
namespace ge
{
class OpenglRendererAPI : public RendererAPI
{
public:
  OpenglRendererAPI()  = default;
  ~OpenglRendererAPI() = default;

  virtual void SetClearColour(glm::vec3& colour) override;
};
} // namespace ge
