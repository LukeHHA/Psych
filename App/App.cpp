#include "AppLayer.h"
#include "Core/GameEngine.h"

#define GE_ENTRYPOINT_IMPL
#include "EntryPoint.h"
#include "ge/Core/Core.h"

class App : public ge::GameEngine
{
public:
  App() : ge::GameEngine()
  {
    CORE_PROFILE_FUNCTION();
    CORE_LOG_INFO("Initializing App");
  }

  ~App()
  {
    CORE_PROFILE_FUNCTION();
    CORE_LOG_INFO("App Shutdown successfully");
  }
};

ge::Expected<ge::Unique<ge::GameEngine>, ge::errors::EngineError>
ge::CreateGameEngine()
{
  CORE_PROFILE_FUNCTION();
  auto app    = ge::CreateUnique<App>();
  auto result = app->Init();
  if (!result) {
    return ge::Unexpected(result.error());
  }

  app->PushLayer(ge::CreateUnique<AppLayer>());
  return std::move(app);
}
