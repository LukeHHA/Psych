#include "Window.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "Logging/Logging.h"
#include "Window/EngineWindow.h"
#include "Window/HeadlessWindow.h"

namespace ge
{
class EventHandler;
Expected<Unique<Window>, errors::WindowError> Window::Create(const std::string& title, unsigned int width, unsigned int height, EventHandler& eventHandler)
{
  CORE_PROFILE_FUNCTION();
  Unique<Window> window;

  switch (RendererAPI::Current()) {
  case RendererAPIType::TEST_HEADLESS:
    window = CreateUnique<HeadlessWindow>();
    break;

  case RendererAPIType::OPENGL:
  case RendererAPIType::VULKAN:
  case RendererAPIType::METAL:
    window = CreateUnique<EngineWindow>();
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
