#include "AppLayer.h"
#include "EntryPoint.h"
#include "GameEngine.h"

#include <memory>

class App : public Core::GameEngine {
public:
  App() {
    CORE_PROFILE_FUNCTION();
    PushLayer(std::move(std::make_unique<AppLayer>()));
  }

  ~App() { CORE_PROFILE_FUNCTION(); }
};

std::unique_ptr<Core::GameEngine> Core::CreateGameEngine() {
  CORE_PROFILE_FUNCTION();
  return std::make_unique<App>();
}
