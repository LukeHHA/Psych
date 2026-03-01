#pragma once

#include "Renderer/RendererAPI.h"
namespace ge
{
class OpenglRendererAPI : public RendererAPI
{
public:
  OpenglRendererAPI()          = default;
  virtual ~OpenglRendererAPI() = default;

  virtual void Init() override;
  virtual void SetClearColour(const glm::vec3& colour) override;
  virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height) override;
  virtual void Clear() override;
  void DrawIndexed(const Shared<VertexArray>& vertexArray,
                   uint32_t indexCount) override;
};
} // namespace ge
