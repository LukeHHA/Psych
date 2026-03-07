#pragma once

#include "Core/Base.h"
#include "Debug/Assert.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "glm/vec3.hpp"
#include <cstdint>

namespace ge
{
enum class RendererAPIType { NONE, OPENGL, VULKAN, METAL, TEST_HEADLESS };

class RendererAPI
{
public:
  RendererAPI()          = default;
  virtual ~RendererAPI() = default;

  virtual void Init()    = 0;
  static void SetAPI(RendererAPIType api) { s_RendererAPI = api; }

  static RendererAPIType Current()
  {
    CORE_ASSERT(s_RendererAPI != RendererAPIType::NONE,
                "RendererAPI::Current() called but no API is set!");
    return s_RendererAPI;
  }

  virtual void SetClearColour(const glm::vec3& colour)                  = 0;
  virtual void SetViewPort(const uint32_t x, const uint32_t y,
                           const uint32_t width, const uint32_t height) = 0;
  virtual void Clear()                                                  = 0;
  virtual void DrawIndexed(const Shared<VertexArray>& vertexArray,
                           const uint32_t indexCount)                   = 0;
  static Unique<RendererAPI> Create();

private:
  inline static RendererAPIType s_RendererAPI;
};
} // namespace ge
