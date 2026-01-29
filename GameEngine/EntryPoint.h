#pragma once

#include "Core.h"
#include "Core/GameEngine.h"

extern std::unique_ptr<ge::GameEngine> ge::CreateGameEngine();

int main(int argc, char** argv) {
    /* Ensure that no Logging is attempted befoire this point
     * otherwise a segmentation fault occurs
     */
    ge::Log::Init();
    CORE_LOG_INFO("Core Library Initializing");
    CORE_LOG_INFO("Initialized Core Logger");
    APP_LOG_INFO("Initialized App Logger");
    CORE_LOG_INFO("Core Library Initialized");

    // CORE_LOG_INFO("Core VERSION_MAJOR: {0}", CORE_VERSION_MAJOR);
    // CORE_LOG_INFO("App VERSION_MINOR: {0}", APP_VERSION_MINOR);

    CORE_PROFILE_BEGIN_SESSION("Startup", "CoreProfile-Startup.json");
    auto app = ge::CreateGameEngine();
    CORE_LOG_INFO("App Session Created Successfully");
    CORE_PROFILE_END_SESSION();

    CORE_PROFILE_BEGIN_SESSION("Runtime", "CoreProfile-Runtime.json");
    CORE_ASSERT(app != nullptr, "Application is nullptr on startup");
    app->Run();
    CORE_PROFILE_END_SESSION();

    CORE_PROFILE_BEGIN_SESSION("Shutdown", "CoreProfile-Shutdown.json");

    CORE_PROFILE_END_SESSION();
}
