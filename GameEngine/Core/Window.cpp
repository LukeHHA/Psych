#include "Window.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "Window/EngineWindow.h"
#include "Window/HeadlessWindow.h"

namespace ge
{
class EventHandler;
Shared<Window> Window::Create(const std::string& title, unsigned int width,
                              unsigned int height,
                              Shared<EventHandler> eventHandler)
{
  CORE_PROFILE_FUNCTION();

  switch (RendererAPI::Current()) {
  case RendererAPIType::TEST_HEADLESS:
    return CreateShared<HeadlessWindow>(title, width, height, eventHandler);

  case RendererAPIType::OPENGL:
  case RendererAPIType::VULKAN:
  case RendererAPIType::METAL:
  case RendererAPIType::NONE:
    return CreateShared<EngineWindow>(title, width, height, eventHandler);
  }

  CORE_ASSERT(false, "Unknown Renderer type, Cannot create window!");
  return nullptr;
}
} // namespace ge
