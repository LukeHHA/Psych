#pragma once

#include <cstdint>

#include "../../Shader.h"

namespace ge
{
class OpenglShader : public Shader
{
public:
  explicit OpenglShader(const std::string& name);
  virtual ~OpenglShader() override;

  virtual void Bind() const override;
  virtual void Unbind() const override;
  virtual void SetInt(const std::string& name, int value) override;
  virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
  virtual void SetFloat(const std::string& name, float value) override;
  virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
  virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
  virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
  virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
  const std::string& GetName() const override { return m_ShaderName_; }

  static Expected<Shared<Shader>, errors::ShaderError> Create(const std::string& vertexSrc, const std::string& fragSrc, const std::string& name);

private:
  Expected<void, errors::ShaderError> Compile(const std::string& vertexSrc, const std::string& fragSrc);
  Expected<void, errors::ShaderError> CheckCompileErrors(unsigned int shader, const std::string& type);

private:
  uint32_t m_ShaderID_      = 0;
  std::string m_ShaderName_ = "Unamed_Shader";
};

} // namespace ge
