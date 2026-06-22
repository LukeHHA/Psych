#pragma once

namespace ge::errors
{

enum class WindowError {
  None = 0,
  InitializationFailed,
  WindowAlreadyExists,
  InvalidDimensions,
  InvalidEventHandler,
  NativeWindowCreationFailed,
  ContextCreationFailed,
  ContextInitializationFailed,
  TitleTooLong
};

enum class EngineError {
  None = 0,
  WindowCreationFailed,
  RendererInitializationFailed,
  FramebufferCreationFailed,
  InvalidGameEngineState,
  GameEngineInitializationFailed
};

enum class EventError { None = 0, SomeEventsNotHandled };

enum class FilesystemError {
  None = 0,
  NotInitialized,
  InvalidPath,
  FileNotFound,
  PathExistsWithWrongType,
  DirectoryCreationFailed,
  FileCreationFailed,
  DeleteFailed,
  DirectoryIterationFailed,
  ReadFailed
};

enum class ShaderLibraryError { None = 0, NotFound };

enum class RendererError {
  None = 0,
  UnsupportedAPI,
  AlreadyInitialized,
  APICreationFailed,
  ContextCreationFailed,
  ContextInitializationFailed,
  FramebufferCreationFailed
};

enum class ShaderError {
  None = 0,
  UnsupportedAPI,
  SourceFileNotFound,
  SourceReadFailed,
  CompileFailed,
  LinkFailed,
  DuplicateName,
  NoGraphicsContext
};

} // namespace ge::errors
