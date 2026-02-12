#include "RendererAPI.h"
#include "Core/Base.h"
#include "Renderer/Platform/Opengl/OpenglRendererAPI.h"

namespace ge
{
Unique<RendererAPI> RendererAPI::Create()
{
  switch (s_RendererAPI) {
  case ge::RendererAPIType::OPENGL:
    return CreateUnique<OpenglRendererAPI>();
  }
  CORE_ASSERT(false, "Unknown Renderer API")
  return {};
}
} // namespace ge
