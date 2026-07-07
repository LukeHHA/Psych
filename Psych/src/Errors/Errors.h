#pragma once

#include <cstdint>
namespace psych::errors
{

enum class WindowError : std::uint8_t {
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

enum class EngineError : std::uint8_t {
  None = 0,
  WindowCreationFailed,
  RendererInitializationFailed,
  FramebufferCreationFailed,
  InvalidPsychEngineState,
  PsychEngineInitializationFailed
};

enum class EventError : std::uint8_t { None = 0, SomeEventsNotHandled };

enum class FilesystemError : std::uint8_t {
  None = 0,
  NotInitialized,
  InvalidPath,
  FileNotFound,
  PathExistsWithWrongType,
  DirectoryCreationFailed,
  FileCreationFailed,
  DeleteFailed,
  DirectoryIterationFailed,
  ReadFailed,
  OSPathFail,
  LoadFailed
};

enum class ShaderLibraryError : std::uint8_t { None = 0, NotFound };

enum class RendererError : std::uint8_t {
  None = 0,
  UnsupportedAPI,
  AlreadyInitialized,
  APICreationFailed,
  ContextCreationFailed,
  ContextInitializationFailed,
  FramebufferCreationFailed
};

enum class ShaderError : std::uint8_t { None = 0, UnsupportedAPI, SourceFileNotFound, SourceReadFailed, CompileFailed, LinkFailed, DuplicateName, NoGraphicsContext };

enum class SerializationError : std::uint8_t { None = 0, SerializationFailed, DeserializationFailed };

enum class ProjectError : std::uint8_t { None = 0, InvalidPath, ConfigLoadFailed, ConfigSaveFailed, DirectoryCreationFailed, NoActiveProject };

} // namespace psych::errors
