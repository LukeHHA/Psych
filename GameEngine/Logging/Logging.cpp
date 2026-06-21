#include "Logging.h"
#include "Core/Core.h"
#include "spdlog/common.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <iostream>

namespace ge
{

void Log::Init()
{

  // Ensure logging file is empty

  using COUTColorSink_mt = spdlog::sinks::stdout_color_sink_mt;
  using FileSink_mt      = spdlog::sinks::basic_file_sink_mt;

  s_CoreConsoleLogger_   = spdlog::stdout_color_mt("CORE");
  s_CoreConsoleLogger_->set_pattern("%^[%T] [%n] [%s:%#]: %v%$");
  s_CoreConsoleLogger_->set_level(spdlog::level::trace);

  s_AppConsoleLogger_ = spdlog::stdout_color_mt("APP");
  s_AppConsoleLogger_->set_pattern("%^[%T] [%n] [%s:%#]: %v%$");
  s_AppConsoleLogger_->set_level(spdlog::level::trace);

  s_JSONSink_ = CreateShared<FileSink_mt>("logs/debug_logs.json", true);
  s_JSONSink_->set_level(spdlog::level::trace);
  s_JSONSink_->set_pattern("{\n \"log\": [");

  s_ConsoleSink_ = CreateShared<COUTColorSink_mt>();
  s_ConsoleSink_->set_level(spdlog::level::debug);

  s_CoreMultiLogger_ = CreateShared<spdlog::logger>(
      "CORE",
      spdlog::sinks_init_list{s_ConsoleSink_, s_JSONSink_});
  s_CoreMultiLogger_->set_level(spdlog::level::trace);

  /// Init JSON logger
  /// https://github.com/gabime/spdlog/wiki/Setting-up-JSON-logging-with-spdlog
  /// Set up the opening brace and an array named "log"
  /// we're setting a global format here but as per the docs you can set
  /// this on an individual log as well
  s_CoreMultiLogger_->trace(""); // this initializes the log file with the
                                 // opening brace and the "log" array as above

  // We have some extra formatting on the log level %l below to keep color
  // coding when dumping json to the console and we use a full ISO 8601
  // time/date format
  std::string jsonpattern = {
      // clang-format off
          "{\"time\": \"%Y-%m-%dT%H:%M:%S.%f%z\", "
          "\"name\": \"%n\", "
          "\"level\": \"%^%l%$\", "
          "\"trace\": \"%g:%#\", "
          "\"func\": \"%!\", "
          "\"process\": %P, "
          "\"thread\": %t, "
          "\"message\": \"%v\"},"
      // clang-format on
  };

  s_JSONSink_->set_pattern(jsonpattern);
}

void Log::Shutdown()
{
  std::string jsonlastlogpattern = {
      // clang-format off
          "{\"time\": \"%Y-%m-%dT%H:%M:%S.%f%z\", "
          "\"name\": \"%n\", "
          "\"level\": \"%^%l%$\", "
          "\"trace\": \"%g:%#\", "
          "\"func\": \"%!\", "
          "\"process\": %P, "
          "\"thread\": %t, "
          "\"message\": \"%v\"}"
      // clang-format on
  };

  s_JSONSink_->set_pattern(jsonlastlogpattern);
  s_CoreMultiLogger_->trace("finished.");
  s_JSONSink_->set_pattern("]\n}");
  s_CoreMultiLogger_->trace("");
  spdlog::drop("CORE");
  std::cout << "logging shutdown\n";
}
} // namespace ge
