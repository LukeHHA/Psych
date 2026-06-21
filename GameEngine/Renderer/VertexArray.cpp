#include "VertexArray.h"
#include "Core/Core.h"
#include "Debug/Assert.h"
#include "Renderer/Platform/Opengl/OpenglVertexArray.h"
#include "Renderer/RendererAPI.h"

namespace ge
{
Shared<VertexArray> VertexArray::Create()
{
  switch (RendererAPI::Current()) {

  case ge::RendererAPIType::OPENGL:
  case ge::RendererAPIType::TEST_HEADLESS:
    return CreateShared<OpenglVertexArray>();

  default:
    CORE_ASSERT(false, "Unimplemented Renderer set");
    return {};
  }
}
} // namespace ge
