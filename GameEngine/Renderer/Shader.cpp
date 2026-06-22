#include "Shader.h"
#include "Core/Core.h"
#include "FileSystem/FileSystem.h"
#include "Renderer/Platform/Opengl/OpenglShader.h"
#include "Renderer/RendererAPI.h"
#include "ge_expected"
#include <cstddef>
#include <string>

namespace ge
{
// SHADER
Expected<Shared<Shader>, errors::ShaderError>
Shader::Create(const std::string& vertexSrc, const std::string& fragSrc,
               const std::string& name)
{
  auto vertexSource = util::Filesystem::TryReadFile(vertexSrc);
  if (!vertexSource) {
    if (vertexSource.error() == errors::FilesystemError::FileNotFound) {
      return Unexpected(errors::ShaderError::SourceFileNotFound);
    }
    return Unexpected(errors::ShaderError::SourceReadFailed);
  }

  auto fragmentSource = util::Filesystem::TryReadFile(fragSrc);
  if (!fragmentSource) {
    if (fragmentSource.error() == errors::FilesystemError::FileNotFound) {
      return Unexpected(errors::ShaderError::SourceFileNotFound);
    }
    return Unexpected(errors::ShaderError::SourceReadFailed);
  }

  switch (RendererAPI::Current()) {
  case RendererAPIType::OPENGL:
    return OpenglShader::Create(vertexSource.value(),
                                fragmentSource.value(),
                                name);
    /*
  case RendererAPIType::METAL:
    return CreateShared<MetalShader>(vertexSrc, fragSrc, name);
  case RendererAPIType::VULKAN:
    return CreateShared<VulkanShader>(vertexSrc, fragSrc, name);
    */
  case RendererAPIType::TEST_HEADLESS:
    return OpenglShader::Create(vertexSource.value(),
                                fragmentSource.value(),
                                name);
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

Expected<Shared<Shader>, errors::ShaderError>
ShaderLibrary::Load(const std::string& vertexSrc, const std::string& fragSrc,
                    const std::string& name)
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
} // namespace ge
