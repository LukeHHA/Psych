#include "Shader.h"
#include "Core/Base.h"
#include "FileSystem/FileSystem.h"
#include "Renderer/Platform/Opengl/OpenglShader.h"
#include "Renderer/RendererAPI.h"
#include "ge_expected"
#include <cstddef>
#include <string>

namespace ge
{
// SHADER
Shared<Shader> Shader::Create(const std::string& vertexSrc,
                              const std::string& fragSrc,
                              const std::string& name)
{
  switch (RendererAPI::Current()) {
  case RendererAPIType::OPENGL:
    return CreateShared<OpenglShader>(vertexSrc, fragSrc, name);
    /*
  case RendererAPIType::METAL:
    return CreateShared<MetalShader>(vertexSrc, fragSrc, name);
  case RendererAPIType::VULKAN:
    return CreateShared<VulkanShader>(vertexSrc, fragSrc, name);
    */
  case RendererAPIType::TEST_HEADLESS:
    return CreateShared<OpenglShader>(vertexSrc, fragSrc, name);
  default:
    CORE_ASSERT(false, "No renderer api has been set");
    return {};
  }
}

// SHADER LIBRARY

Shared<ShaderLibrary> ShaderLibrary::Create()
{
  return CreateShared<ShaderLibrary>();
}

void ShaderLibrary::Add(const Shared<Shader>& shader, const std::string& name)
{
  CORE_ASSERT(shader, "{}: cannot add a shader of nullptr to library", name);
  m_Shaders_.insert({name, shader});
}

Shared<Shader> ShaderLibrary::Load(const std::string& vertexSrc,
                                   const std::string& fragSrc,
                                   const std::string& name)
{
  if (util::Filesystem::FileExists(vertexSrc) &&
      util::Filesystem::FileExists(fragSrc)) {
  }

  auto [it, inserted] =
      m_Shaders_.emplace(name, Shader::Create(vertexSrc, fragSrc, name));

  CORE_ASSERT(inserted, "Shader '{}' already exists", name);
  return it->second;
}

util::expected<Shared<Shader>, ShaderLibraryError>
ShaderLibrary::Get(const std::string& name) const
{
  // For now this function will assert if the shader is not found
  // however when the engine compiles as a editor it will need to
  // handle this properly ie. util::expected

  if (auto it = m_Shaders_.find(name); it != m_Shaders_.end()) {
    return it->second;
  } else {
    return util::unexpected(ShaderLibraryError{ShaderLibraryErrors::NotFound});
  }
}

bool ShaderLibrary::Exists(const std::string& name) const
{
  return m_Shaders_.contains(name);
}
} // namespace ge
