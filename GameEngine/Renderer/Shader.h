#pragma once

#include "Core/Core.h"
#include "Errors/Errors.h"
#include "ge_expected"
#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

namespace ge
{
class Shader
{
public:
  Shader()                                                                       = default;
  virtual ~Shader()                                                              = default;
  virtual void Bind() const                                                      = 0;
  virtual void Unbind() const                                                    = 0;
  virtual void SetInt(const std::string& name, int value)                        = 0;
  virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
  virtual void SetFloat(const std::string& name, float value)                    = 0;
  virtual void SetFloat2(const std::string& name, const glm::vec2& value)        = 0;
  virtual void SetFloat3(const std::string& name, const glm::vec3& value)        = 0;
  virtual void SetFloat4(const std::string& name, const glm::vec4& value)        = 0;
  virtual void SetMat4(const std::string& name, const glm::mat4& value)          = 0;
  virtual const std::string& GetName() const                                     = 0;

  static Shared<Shader> Create(const std::string& vertexSrc, const std::string& fragSrc, const std::string& name);
};

class ShaderLibrary
{
public:
  ShaderLibrary()  = default;
  ~ShaderLibrary() = default;
  void Add(const Shared<Shader>& shader, const std::string& name);
  Shared<Shader> Load(const std::string& vertexSrc, const std::string& fragSrc, const std::string& name);
  Expected<Shared<Shader>, errors::ShaderLibraryError> Get(const std::string& name) const;
  bool Exists(const std::string& name) const;

  static Shared<ShaderLibrary> Create();

private:
  std::unordered_map<std::string, Shared<Shader>> m_Shaders_;
};
} // namespace ge
