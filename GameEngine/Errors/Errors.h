#pragma once

#include <string>

namespace ge::errors {
    enum class WindowError {
        None = 0,
        InitializationFailed,
        WindowAlreadyExists,
        InvalidDimensions,
        TitleTooLong
    };

    struct WindowErrorCategory {
        WindowError code;
        std::string message;
    };

    enum class EngineError {
        None = 0,
        WindowCreationFailed,
        RendererInitializationFailed,
        InvalidGameEngineState,
        GameEngineInitializationFailed
    };

    struct EngineErrorCategory {
        EngineError code;
        std::string message;
    };
}