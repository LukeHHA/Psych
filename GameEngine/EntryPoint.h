#pragma once

#include <iostream>
#include <memory>

#include "Core/GameEngine.h"
#include "Debug/Instrumentor.h"
#include "FileSystem/FileSystem.h"
#include "Renderer/RendererAPI.h"
#include "Util/CommandLine.h"

// MAIN
extern ge::Expected<std::unique_ptr<ge::GameEngine>, ge::errors::EngineError> ge::CreateGameEngine(ge::GameEngineSpecification& spec);

int main(int argc, char** argv)
{
  {
    const ge::cli::ParseResult parseResult = ge::cli::CommandLineParser::Parse(argc, argv);

    if (!parseResult.Success) {
      std::cerr << parseResult.ErrorMessage;
      return 1;
    }

    if (parseResult.Options.ShowHelp) {
      std::cout << ge::cli::CommandLineParser::HelpText();
      return 0;
    }

    ge::Log::Init();
    ge::util::Filesystem::Init();

    ge::GameEngineSpecification spec;
    spec.Name          = "Game Engine";
    spec.RenderingAPI  = parseResult.Options.RenderingAPI;
    spec.AssetBasePath = parseResult.Options.DataDirectory;

    CORE_PROFILE_BEGIN_SESSION("Startup", "CoreProfile-Startup.json");
    auto appResult = ge::CreateGameEngine(spec);
    if (!appResult) {
      std::cerr << "Failed to create game engine\n";
      return 1;
    }

    auto app = std::move(appResult.value());
    CORE_LOG_INFO("App Session Created Successfully");
    CORE_PROFILE_END_SESSION();

    CORE_PROFILE_BEGIN_SESSION("Runtime", "CoreProfile-Runtime.json");
    CORE_ASSERT(app != nullptr, "Application is nullptr on startup");
    app->Run();
    CORE_PROFILE_END_SESSION();

    CORE_PROFILE_BEGIN_SESSION("Shutdown", "CoreProfile-Shutdown.json");
    auto shutdownResult = app->Shutdown();
    CORE_PROFILE_END_SESSION();
    if (!shutdownResult) {
      std::cerr << "Failed to shutdown game engine\n";
      ge::util::Filesystem::Shutdown();
      return 1;
    }

    ge::util::Filesystem::Shutdown();
    CORE_LOG_INFO("Filesystem Shutdown");
  }

  return 0;
}
