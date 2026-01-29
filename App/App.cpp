#include "AppLayer.h"
#include "EntryPoint.h"

#include <memory>

class App : public ge::GameEngine {
public:
    App() {
        CORE_PROFILE_FUNCTION();
        PushLayer(std::move(std::make_unique<AppLayer>()));
    }

    ~App() { CORE_PROFILE_FUNCTION(); }
};

std::unique_ptr<ge::GameEngine> ge::CreateGameEngine() {
    CORE_PROFILE_FUNCTION();
    return std::make_unique<App>();
}
