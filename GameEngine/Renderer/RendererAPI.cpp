#include "RendererAPI.h"
#include "Core/Base.h"
#include "Renderer/Platform/Opengl/OpenglRendererAPI.h"

namespace ge
{
Unique<RendererAPI> RendererAPI::Create()
{
  switch (s_RendererAPI) {
  case ge::RendererAPIType::OPENGL:
  case ge::RendererAPIType::TEST_HEADLESS:
    return CreateUnique<OpenglRendererAPI>();
  case ge::RendererAPIType::NONE:
    CORE_ASSERT(false, "No API type has been set")
  }

  CORE_ASSERT(false, "Unknown Renderer API")
  return {};
}
} // namespace ge
