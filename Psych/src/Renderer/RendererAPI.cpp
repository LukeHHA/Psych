#include "RendererAPI.h"
#include "Core/Core.h"
#include "Renderer/Platform/Opengl/OpenglRendererAPI.h"

namespace psych
{
Expected<Unique<RendererAPI>, errors::RendererError> RendererAPI::Create()
{
  switch (s_RendererAPI) {
  case psych::RendererAPIType::OPENGL:
  case psych::RendererAPIType::TEST_HEADLESS:
    return CreateUnique<OpenglRendererAPI>();
  case psych::RendererAPIType::NONE:
    return Unexpected(errors::RendererError::UnsupportedAPI);
  case psych::RendererAPIType::VULKAN:
  case psych::RendererAPIType::METAL:
    return Unexpected(errors::RendererError::UnsupportedAPI);
  }

  return Unexpected(errors::RendererError::UnsupportedAPI);
}
} // namespace psych
