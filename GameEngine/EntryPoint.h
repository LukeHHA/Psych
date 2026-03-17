#pragma once

#include <iostream>
#include <memory>

#include "Core/GameEngine.h"
#include "Debug/Instrumentor.h"
#include "FileSystem/FileSystem.h"
#include "Renderer/RendererAPI.h"
#include "Util/CommandLine.h"

extern std::unique_ptr<ge::GameEngine>
ge::CreateGameEngine(ge::GameEngineSpecification& spec);

int main(int argc, char** argv)
{
  {
    const ge::cli::ParseResult parseResult =
        ge::cli::CommandLineParser::Parse(argc, argv);

    if (!parseResult.Success) {
      std::cerr << parseResult.ErrorMessage;
      return 1;
    }

    if (parseResult.Options.ShowHelp) {
      std::cout << ge::cli::CommandLineParser::HelpText();
      return 0;
    }

    ge::util::Filesystem::Init();
    ge::Log::Init();

    CORE_LOG_INFO("Logging Init");
    CORE_LOG_INFO("Filesystem Init");

    ge::GameEngineSpecification spec;
    spec.Name          = "Game Engine";
    spec.RenderingAPI  = parseResult.Options.RenderingAPI;
    spec.AssetBasePath = parseResult.Options.DataDirectory;

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
  }

  return 0;
}
