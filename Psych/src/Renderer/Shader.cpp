 
/**************************************************************************/
/*  Shader.cpp                                                            */                                                            
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

#include "Shader.h"
#include "Core/Core.h"
#include "FileSystem/EngineFilesystem.h"
#include "FileSystem/FileSystem.h"
#include "Renderer/Platform/Opengl/OpenglShader.h"
#include "Renderer/RendererAPI.h"
#include "expected.h"
#include <cstddef>
#include <string>

namespace psych
{
namespace
{
Expected<std::string, errors::FilesystemError> TryReadShaderSource(const std::string& path)
{
  if (path.find("://") != std::string::npos) {
    return util::EngineFilesystem::TryReadFile(path);
  }

  return util::Filesystem::TryReadFile(path);
}
} // namespace

// SHADER
Expected<Shared<Shader>, errors::ShaderError> Shader::Create(const std::string& vertexSrc, const std::string& fragSrc, const std::string& name)
{
  auto vertexSource = TryReadShaderSource(vertexSrc);
  if (!vertexSource) {
    if (vertexSource.error() == errors::FilesystemError::FileNotFound) {
      return Unexpected(errors::ShaderError::SourceFileNotFound);
    }
    return Unexpected(errors::ShaderError::SourceReadFailed);
  }

  auto fragmentSource = TryReadShaderSource(fragSrc);
  if (!fragmentSource) {
    if (fragmentSource.error() == errors::FilesystemError::FileNotFound) {
      return Unexpected(errors::ShaderError::SourceFileNotFound);
    }
    return Unexpected(errors::ShaderError::SourceReadFailed);
  }

  switch (RendererAPI::Current()) {
  case RendererAPIType::OPENGL:
    return OpenglShader::Create(vertexSource.value(), fragmentSource.value(), name);
    /*
  case RendererAPIType::METAL:
    return CreateShared<MetalShader>(vertexSrc, fragSrc, name);
  case RendererAPIType::VULKAN:
    return CreateShared<VulkanShader>(vertexSrc, fragSrc, name);
    */
  case RendererAPIType::TEST_HEADLESS:
    return OpenglShader::Create(vertexSource.value(), fragmentSource.value(), name);
  default:
    return Unexpected(errors::ShaderError::UnsupportedAPI);
  }
}

// SHADER LIBRARY

Shared<ShaderLibrary> ShaderLibrary::Create() { return CreateShared<ShaderLibrary>(); }

void ShaderLibrary::Add(const Shared<Shader>& shader, const std::string& name)
{
  CORE_ASSERT(shader, "{}: cannot add a shader of nullptr to library", name);
  m_Shaders_.insert({name, shader});
}

Expected<Shared<Shader>, errors::ShaderError> ShaderLibrary::Load(const std::string& vertexSrc, const std::string& fragSrc, const std::string& name)
{
  if (m_Shaders_.contains(name)) {
    return Unexpected(errors::ShaderError::DuplicateName);
  }

  auto shader = Shader::Create(vertexSrc, fragSrc, name);
  if (!shader) {
    return Unexpected(shader.error());
  }

  auto [it, inserted] = m_Shaders_.emplace(name, shader.value());
  if (!inserted) {
    return Unexpected(errors::ShaderError::DuplicateName);
  }
  return it->second;
}

Expected<Shared<Shader>, errors::ShaderLibraryError> ShaderLibrary::Get(const std::string& name) const
{
  // For now this function will assert if the shader is not found
  // however when the engine compiles as a editor it will need to
  // handle this properly ie. Expected

  if (auto it = m_Shaders_.find(name); it != m_Shaders_.end()) {
    return it->second;
  } else {
    return Unexpected(errors::ShaderLibraryError::NotFound);
  }
}

bool ShaderLibrary::Exists(const std::string& name) const { return m_Shaders_.contains(name); }
} // namespace psych
