#pragma once

namespace ge::errors
{

enum class WindowError { None = 0, InitializationFailed, WindowAlreadyExists, InvalidDimensions, TitleTooLong };

enum class EngineError {
  None = 0,
  WindowCreationFailed,
  RendererInitializationFailed,
  InvalidGameEngineState,
  GameEngineInitializationFailed
};

enum class EventError { None = 0, SomeEventsNotHandled };

enum class FilesystemError {
  None = 0,
  NotInitialized,
  InvalidPath,
  PathExistsWithWrongType,
  DirectoryCreationFailed,
  FileCreationFailed,
  ReadFailed
};

enum class ShaderLibraryError { None = 0, NotFound };

} // namespace ge::errors
