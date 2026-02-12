#pragma once

#include "Core/Base.h"
#include "glm/vec3.hpp"
#include <Debug/Assert.h>

namespace ge
{
enum class RendererAPIType { NONE, OPENGL, VULKAN, METAL, TEST_HEADLESS };

class RendererAPI
{
public:
  RendererAPI()          = default;
  virtual ~RendererAPI() = default;
  static void SetAPI(RendererAPIType api) { s_RendererAPI = api; }

  static RendererAPIType Current()
  {
    CORE_ASSERT(s_RendererAPI != RendererAPIType::NONE,
                "RendererAPI::Current() called but no API is set!");
    return s_RendererAPI;
  }

  virtual void SetClearColour(const glm::vec3& colour) = 0;
  virtual void Clear()                                 = 0;
  static Unique<RendererAPI> Create();

private:
  inline static RendererAPIType s_RendererAPI;
};
} // namespace ge
