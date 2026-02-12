#pragma once

#include "Core/Base.h"
#include <Debug/Assert.h>
#include <glm/vec3.hpp>

namespace ge
{
enum class RendererAPIType { NONE, OPENGL, VULKAN, METAL, TEST_HEADLESS };

class RendererAPI
{
public:
  static RendererAPIType SetAPI(RendererAPIType api)
  {
    s_RendererAPI = api;
    return s_RendererAPI;
  }

  static RendererAPIType Current()
  {
    CORE_ASSERT(s_RendererAPI != RendererAPIType::NONE,
                "RendererAPI::Current() called but no API is set!");
    return s_RendererAPI;
  }

  virtual void SetClearColour(glm::vec3& colour) = 0;
  static Unique<RendererAPI> Create();

private:
  inline static RendererAPIType s_RendererAPI;
};
} // namespace ge
