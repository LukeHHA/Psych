#pragma once

#include "Renderer/RendererAPI.h"
namespace ge
{
class OpenglRendererAPI : public RendererAPI
{
public:
  OpenglRendererAPI()          = default;
  virtual ~OpenglRendererAPI() = default;

  virtual Expected<void, errors::RendererError> Init() override;
  virtual void SetClearColour(const glm::vec3& colour) override;
  virtual void SetViewPort(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) override;
  virtual void Clear() override;
  void DrawIndexed(const Shared<VertexArray>& vertexArray, const uint32_t indexCount) override;
};
} // namespace ge
