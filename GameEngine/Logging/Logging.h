#pragma once

#include "Core/Base.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace ge
{
class Log
{
public:
  Log()  = delete;
  ~Log() = delete;
  CORE_NO_COPY_NO_MOVE(Log);
  static void Init();
  static void Shutdown();

  inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
  {
    return s_CoreConsoleLogger_;
  }

  inline static std::shared_ptr<spdlog::logger>& GetAppLogger()
  {
    return s_AppConsoleLogger_;
  }

  inline static Shared<spdlog::logger>& GetMultiLogger()
  {
    return s_CoreMultiLogger_;
  }

private:
  inline static Shared<spdlog::logger> s_CoreConsoleLogger_;
  inline static Shared<spdlog::logger> s_AppConsoleLogger_;
  inline static Shared<spdlog::logger> s_CoreMultiLogger_;
  inline static Shared<spdlog::sinks::basic_file_sink_mt> s_JSONSink_;
  inline static Shared<spdlog::sinks::stdout_color_sink_mt> s_ConsoleSink_;
};
} // namespace ge

// clang-format off
// Core Multi Logger
#define CORE_LOGGER ::ge::Log::GetMultiLogger()
#define CORE_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(CORE_LOGGER, __VA_ARGS__)
#define CORE_LOG_WARN(...) SPDLOG_LOGGER_WARN(CORE_LOGGER, __VA_ARGS__)
#define CORE_LOG_INFO(...) SPDLOG_LOGGER_INFO(CORE_LOGGER, __VA_ARGS__)
#define CORE_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(CORE_LOGGER, __VA_ARGS__)
#define CORE_LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(CORE_LOGGER, __VA_ARGS__)

// App Logger
#define APP_LOGGER ::ge::Log::GetAppLogger()
#define APP_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(APP_LOGGER, __VA_ARGS__)
#define APP_LOG_WARN(...) SPDLOG_LOGGER_WARN(APP_LOGGER, __VA_ARGS__)
#define APP_LOG_INFO(...) SPDLOG_LOGGER_INFO(APP_LOGGER, __VA_ARGS__)
#define APP_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(APP_LOGGER, __VA_ARGS__)
#define APP_LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(APP_LOGGER, __VA_ARGS__)

// Debug trace
// clang-format on
