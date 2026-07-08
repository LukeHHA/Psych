 
/**************************************************************************/
/*  Errors.h                                                              */                                                            
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
