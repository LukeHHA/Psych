#include "VertexArray.h"
#include "Core/Core.h"
#include "Debug/Assert.h"
#include "Renderer/Platform/Opengl/OpenglVertexArray.h"
#include "Renderer/RendererAPI.h"

namespace psych
{
Shared<VertexArray> VertexArray::Create()
{
  switch (RendererAPI::Current()) {

  case psych::RendererAPIType::OPENGL:
  case psych::RendererAPIType::TEST_HEADLESS:
    return CreateShared<OpenglVertexArray>();

  default:
    CORE_ASSERT(false, "Unimplemented Renderer set");
    return {};
  }
}
} // namespace psych
