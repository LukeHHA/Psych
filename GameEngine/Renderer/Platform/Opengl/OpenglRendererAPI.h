#pragma once

#include "Renderer/RendererAPI.h"
namespace ge
{
class OpenglRendererAPI : public RendererAPI
{
public:
  OpenglRendererAPI()          = default;
  virtual ~OpenglRendererAPI() = default;

  virtual void SetClearColour(const glm::vec3& colour) override;
  virtual void Clear() override;
};
} // namespace ge
