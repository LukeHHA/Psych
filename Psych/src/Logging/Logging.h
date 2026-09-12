 
/**************************************************************************/
/*  Logging.h                                                             */                                                            
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

#include "Core/Core.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace psych
{
class Log
{
public:
  Log()  = delete;
  ~Log() = delete;
  CORE_NO_COPY_NO_MOVE(Log);
  static void Init();
  static void Shutdown();

  inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreConsoleLogger_; }

  inline static std::shared_ptr<spdlog::logger>& GetAppLogger() { return s_AppConsoleLogger_; }

  inline static Shared<spdlog::logger>& GetMultiLogger() { return s_CoreMultiLogger_; }

private:
  inline static Shared<spdlog::logger> s_CoreConsoleLogger_;
  inline static Shared<spdlog::logger> s_AppConsoleLogger_;
  inline static Shared<spdlog::logger> s_CoreMultiLogger_;
  inline static Shared<spdlog::sinks::basic_file_sink_mt> s_JSONSink_;
  inline static Shared<spdlog::sinks::stdout_color_sink_mt> s_ConsoleSink_;
};
} // namespace psych

// clang-format off
// Core Multi Logger
#define CORE_LOGGER ::psych::Log::GetMultiLogger()
#define CORE_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(CORE_LOGGER, __VA_ARGS__)
#define CORE_LOG_WARN(...) SPDLOG_LOGGER_WARN(CORE_LOGGER, __VA_ARGS__)
#define CORE_LOG_INFO(...) SPDLOG_LOGGER_INFO(CORE_LOGGER, __VA_ARGS__)
#define CORE_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(CORE_LOGGER, __VA_ARGS__)
#define CORE_LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(CORE_LOGGER, __VA_ARGS__)

// App Logger
#define APP_LOGGER ::psych::Log::GetAppLogger()
#define APP_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(APP_LOGGER, __VA_ARGS__)
#define APP_LOG_WARN(...) SPDLOG_LOGGER_WARN(APP_LOGGER, __VA_ARGS__)
#define APP_LOG_INFO(...) SPDLOG_LOGGER_INFO(APP_LOGGER, __VA_ARGS__)
#define APP_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(APP_LOGGER, __VA_ARGS__)
#define APP_LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(APP_LOGGER, __VA_ARGS__)

// Debug trace
// clang-format on
