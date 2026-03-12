#include "AppLayer.h"
#include "Core/GameEngine.h"

#define GE_ENTRYPOINT_IMPL
#include "EntryPoint.h"

#include <memory>

class App : public ge::GameEngine
{
public:
  App(ge::GameEngineSpecification& spec) : ge::GameEngine(spec)
  {
    CORE_PROFILE_FUNCTION();
    CORE_LOG_INFO("Initializing App");
    PushLayer(std::make_unique<AppLayer>());
  }

  ~App()
  {
    CORE_PROFILE_FUNCTION();
    CORE_LOG_INFO("App Shutdown successfully");
  }
};

std::unique_ptr<ge::GameEngine>
ge::CreateGameEngine(ge::GameEngineSpecification& spec)
{
  CORE_PROFILE_FUNCTION();
  return std::make_unique<App>(spec);
}
