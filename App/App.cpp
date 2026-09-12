#include "AppLayer.h"
#include "Core/PsychEngine.h"

#define PSYCH_ENTRYPOINT_IMPL
#include "EntryPoint.h"
#include "Core/Core.h"

class App : public psych::PsychEngine
{
public:
  App() : psych::PsychEngine()
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

psych::Expected<psych::Unique<psych::PsychEngine>, psych::errors::EngineError> psych::CreatePsychEngine()
{
  CORE_PROFILE_FUNCTION();
  auto app    = psych::CreateUnique<App>();
  auto result = app->Init();
  if (!result) {
    return psych::Unexpected(result.error());
  }

  app->PushLayer(psych::CreateUnique<AppLayer>());
  return std::move(app);
}
