#include "Renderer/RendererContext.h"
#include "Renderer/Platform/Opengl/OpenglContext.h"
#include "Renderer/Platform/Vulkan/VulkanContext.h"
#include "Renderer/RendererAPI.h"
#include <Debug/Instrumentor.h>

namespace ge
{
Expected<Shared<RendererContext>, errors::RendererError> RendererContext::Create(GLFWwindow* window)
{
  CORE_PROFILE_FUNCTION();

  if (window == nullptr) {
    return Unexpected(errors::RendererError::ContextCreationFailed);
  }

  switch (RendererAPI::Current()) {
  case RendererAPIType::NONE:
    return Unexpected(errors::RendererError::UnsupportedAPI);
  case RendererAPIType::TEST_HEADLESS:
    return CreateShared<RendererContextHeadless>(window);
  case RendererAPIType::OPENGL:
    return CreateShared<OpenglContext>(window);
  case RendererAPIType::VULKAN:
    return CreateShared<VulkanContext>();
  case RendererAPIType::METAL:
    return CreateShared<OpenglContext>(window);
  default:
    return Unexpected(errors::RendererError::UnsupportedAPI);
  }
}
} // namespace ge
