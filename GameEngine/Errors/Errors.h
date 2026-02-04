#pragma once

#include <string>

namespace ge::errors {

    template <class C>
    struct ErrorInfo {
        ErrorInfo() = default;
        ErrorInfo(std::string msg) : message(msg) {}
        C ErrorCodes;
        std::string message;
    };

    enum class WindowError {
        None = 0,
        InitializationFailed,
        WindowAlreadyExists,
        InvalidDimensions,
        TitleTooLong
    };

    enum class EngineError {
        None = 0,
        WindowCreationFailed,
        RendererInitializationFailed,
        InvalidGameEngineState,
        GameEngineInitializationFailed
    };

    enum class EventError {
        None = 0,
        SomeEventsNotHandled
    };

    template <class CodeEnum>
    using ErrorCategory = ErrorInfo<CodeEnum>;

    using WindowErrors = ErrorCategory<WindowError>;
    using EngineErrors = ErrorCategory<EngineError>;
    using EventErrors = ErrorCategory<EventError>;

} // namespace ge::errors