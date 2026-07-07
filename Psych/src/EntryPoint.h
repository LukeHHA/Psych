#pragma once

#include <iostream>
#include <memory>

#include "Core/PsychEngine.h"
#include "Debug/Instrumentor.h"
#include "FileSystem/FileSystem.h"
#include "Util/CommandLine.h"

// MAIN
extern psych::Expected<std::unique_ptr<psych::PsychEngine>, psych::errors::EngineError> psych::CreatePsychEngine();

int main(int argc, char** argv)
{
  {
    const psych::cli::ParseResult parseResult = psych::cli::CommandLineParser::Parse(argc, argv);

    if (!parseResult.Success) {
      std::cerr << parseResult.ErrorMessage;
      return 1;
    }

    if (parseResult.Options.ShowHelp) {
      std::cout << psych::cli::CommandLineParser::HelpText();
      return 0;
    }

    psych::Log::Init();
    psych::util::Filesystem::Init();

    CORE_PROFILE_BEGIN_SESSION("Startup", "CoreProfile-Startup.json");
    auto appResult = psych::CreatePsychEngine();
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
      psych::util::Filesystem::Shutdown();
      return 1;
    }

    psych::util::Filesystem::Shutdown();
    CORE_LOG_INFO("Filesystem Shutdown");
  }

  return 0;
}
