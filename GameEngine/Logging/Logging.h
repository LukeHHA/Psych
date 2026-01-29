#pragma once

#include <memory>

#include "spdlog/spdlog.h"

namespace ge {
    class Log {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
            return s_CoreLogger;
        }

        inline static std::shared_ptr<spdlog::logger>& GetAppLogger() {
            return s_AppLogger;
        }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_AppLogger;
    };
} // namespace ge

// Core Logger
#define CORE_LOG_ERROR(...) ::ge::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_LOG_WARN(...) ::ge::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOG_INFO(...) ::ge::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_LOG_TRACE(...) ::ge::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_LOG_CRITICAL(...) ::ge::Log::GetCoreLogger()->critical(__VA_ARGS__)
// App Logger
#define APP_LOG_ERROR(...) ::ge::Log::GetAppLogger()->error(__VA_ARGS__)
#define APP_LOG_WARN(...) ::ge::Log::GetAppLogger()->warn(__VA_ARGS__)
#define APP_LOG_INFO(...) ::ge::Log::GetAppLogger()->info(__VA_ARGS__)
#define APP_LOG_TRACE(...) ::ge::Log::GetAppLogger()->trace(__VA_ARGS__)
#define APP_LOG_CRITICAL(...) ::ge::Log::GetAppLogger()->critical(__VA_ARGS__)