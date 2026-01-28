#pragma once

#include <memory>

#include "spdlog/spdlog.h"

namespace Core
{
    class Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger> &GetCoreLogger()
        {
            return s_CoreLogger;
        }

        inline static std::shared_ptr<spdlog::logger> &GetAppLogger()
        {
            return s_AppLogger;
        }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_AppLogger;
    };
}

// Core Logger
#define CORE_LOG_ERROR(...) ::Core::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_LOG_WARN(...) ::Core::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOG_INFO(...) ::Core::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_LOG_TRACE(...) ::Core::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_LOG_CRITICAL(...) ::Core::Log::GetCoreLogger()->critical(__VA_ARGS__)

// App Logger
#define APP_LOG_ERROR(...) ::Core::Log::GetAppLogger()->error(__VA_ARGS__)
#define APP_LOG_WARN(...) ::Core::Log::GetAppLogger()->warn(__VA_ARGS__)
#define APP_LOG_INFO(...) ::Core::Log::GetAppLogger()->info(__VA_ARGS__)
#define APP_LOG_TRACE(...) ::Core::Log::GetAppLogger()->trace(__VA_ARGS__)
#define APP_LOG_CRITICAL(...) ::Core::Log::GetAppLogger()->critical(__VA_ARGS__)