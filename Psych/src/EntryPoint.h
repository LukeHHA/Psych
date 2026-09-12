
/**************************************************************************/
/*  EntryPoint.h                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PSYCH ENGINE                               */
/**************************************************************************/
/* Copyright (c)  Luke Howe                                               */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include <iostream>
#include <memory>

#include "Core/PsychEngine.h"
#include "Debug/Instrumentor.h"
#include "FileSystem/FileSystem.h"
#include "OS/OS.h"
#include "Util/CommandLine.h"

// MAIN
extern psych::Expected<std::unique_ptr<psych::PsychEngine>, psych::errors::EngineError> psych::CreatePsychEngine();

int main(int argc, char** argv)
{
  {
    // const psych::cli::ParseResult parseResult = psych::cli::CommandLineParser::Parse(argc, argv);
    //
    // if (!parseResult.Success) {
    //   std::cerr << parseResult.ErrorMessage;
    //   return 1;
    // }
    //
    // if (parseResult.Options.ShowHelp) {
    //   std::cout << psych::cli::CommandLineParser::HelpText();
    //   return 0;
    // }

    psych::OS::Init();
    psych::Filesystem::Init();
    psych::Log::Init();

    CORE_PROFILE_BEGIN_SESSION("Startup", "CoreProfile-Startup.json");
    auto appResult = psych::CreatePsychEngine();
    if (!appResult) {
      std::cerr << "Failed to create Psych engine\n";
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
      std::cerr << "Failed to shutdown Psych engine\n";
      psych::Filesystem::Shutdown();
      return 1;
    }

    psych::Filesystem::Shutdown();
    psych::OS::Shutdown();
    CORE_LOG_INFO("Filesystem Shutdown");
  }

  return 0;
}
