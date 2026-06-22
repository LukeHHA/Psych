#include "Window.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "Logging/Logging.h"
#include "Window/EngineWindow.h"
#include "Window/HeadlessWindow.h"

namespace ge
{
class EventHandler;
Expected<Shared<Window>, errors::WindowError>
Window::Create(const std::string& title, unsigned int width,
               unsigned int height, Shared<EventHandler> eventHandler)
{
  CORE_PROFILE_FUNCTION();
  Shared<Window> window;

  switch (RendererAPI::Current()) {
  case RendererAPIType::TEST_HEADLESS:
    window = CreateShared<HeadlessWindow>();
    break;

  case RendererAPIType::OPENGL:
  case RendererAPIType::VULKAN:
  case RendererAPIType::METAL:
    window = CreateShared<EngineWindow>();
    break;
  case RendererAPIType::NONE:
    return Unexpected(errors::WindowError::InitializationFailed);
  }

  if (!window) {
    return Unexpected(errors::WindowError::InitializationFailed);
  }

  const auto result = window->Init(title, width, height, eventHandler);
  if (!result) {
    return Unexpected(result.error());
  }

  return window;
}
} // namespace ge
