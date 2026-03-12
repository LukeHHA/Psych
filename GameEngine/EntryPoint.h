#pragma once

#include "Core.h"
#include "Core/GameEngine.h"
#include "FileSystem/FileSystem.h"
#include "Renderer/RendererAPI.h"

extern std::unique_ptr<ge::GameEngine>
ge::CreateGameEngine(ge::GameEngineSpecification& spec);

int main(int argc, char** argv)
{
  /* Ensure that no Logging is attempted befoire this point
   * otherwise a segmentation fault occurs
   */
  {
    ge::util::Filesystem::Init();
    ge::Log::Init();
    CORE_LOG_INFO("Logging Init");
    CORE_LOG_INFO("Filesystem Init");

    ge::GameEngineSpecification spec;
    spec.Name         = "Headless Build";
    spec.RenderingAPI = ge::RendererAPIType::TEST_HEADLESS;

    CORE_PROFILE_BEGIN_SESSION("Startup", "CoreProfile-Startup.json");
    auto app = ge::CreateGameEngine(spec);
    CORE_LOG_INFO("App Session Created Successfully");
    CORE_PROFILE_END_SESSION();

    CORE_PROFILE_BEGIN_SESSION("Runtime", "CoreProfile-Runtime.json");
    CORE_ASSERT(app != nullptr, "Application is nullptr on startup");
    app->Run();
    CORE_PROFILE_END_SESSION();

    CORE_PROFILE_BEGIN_SESSION("Shutdown", "CoreProfile-Shutdown.json");
    CORE_PROFILE_END_SESSION();

    ge::util::Filesystem::Shutdown();
    CORE_LOG_INFO("Filesystem Shutdown");
  } // Scoped to ensure that logging is the last thing to destruct

  ge::Log::Shutdown();

  return 0;
}
