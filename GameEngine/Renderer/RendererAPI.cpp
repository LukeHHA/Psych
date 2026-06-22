#include "RendererAPI.h"
#include "Core/Core.h"
#include "Renderer/Platform/Opengl/OpenglRendererAPI.h"

namespace ge
{
Expected<Unique<RendererAPI>, errors::RendererError> RendererAPI::Create()
{
  switch (s_RendererAPI) {
  case ge::RendererAPIType::OPENGL:
  case ge::RendererAPIType::TEST_HEADLESS:
    return CreateUnique<OpenglRendererAPI>();
  case ge::RendererAPIType::NONE:
    return Unexpected(errors::RendererError::UnsupportedAPI);
  case ge::RendererAPIType::VULKAN:
  case ge::RendererAPIType::METAL:
    return Unexpected(errors::RendererError::UnsupportedAPI);
  }

  return Unexpected(errors::RendererError::UnsupportedAPI);
}
} // namespace ge
