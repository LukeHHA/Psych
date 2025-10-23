#include "Core.h"
#include "AppLayer.h"
#include "EntryPoint.h"

#include <memory>

class App : public Core::Application
{
public:
    App()
    {
        CORE_PROFILE_FUNCTION();
        PushLayer(std::move(std::make_unique<AppLayer>()));
        // PushLayer(new TestOverlay());
    }

    ~App()
    {
        CORE_PROFILE_FUNCTION();
    }
};

std::unique_ptr<Core::Application> Core::CreateApplication()
{
    CORE_PROFILE_FUNCTION();
    return std::make_unique<App>();
}