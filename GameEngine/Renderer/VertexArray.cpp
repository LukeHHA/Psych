#include "VertexArray.h"
#include "Core/Base.h"
#include "Debug/Assert.h"
#include "Renderer/Platform/Opengl/OpenglVertexArray.h"
#include "Renderer/RendererAPI.h"

namespace ge
{
Shared<VertexArray> VertexArray::Create()
{
  switch (RendererAPI::Current()) {

  case ge::RendererAPIType::OPENGL:
    return CreateShared<OpenglVertexArray>();

  default:
    CORE_ASSERT(false, "Unimplemented Renderer set");
    return {};
  }
}
} // namespace ge
