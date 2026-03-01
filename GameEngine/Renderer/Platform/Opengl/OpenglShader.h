#pragma once

#include <cstdint>

#include "../../Shader.h"

namespace ge
{
class OpenglShader : public Shader
{
public:
  OpenglShader(const std::string& vertexSrc, const std::string& fragSrc,
               const std::string& name);
  ~OpenglShader() override;

  virtual void Bind() const override;
  virtual void Unbind() const override;
  virtual void SetInt(const std::string& name, int value) override;
  virtual void SetIntArray(const std::string& name, int* values,
                           uint32_t count) override;
  virtual void SetFloat(const std::string& name, float value) override;
  virtual void SetFloat2(const std::string& name,
                         const glm::vec2& value) override;
  virtual void SetFloat3(const std::string& name,
                         const glm::vec3& value) override;
  virtual void SetFloat4(const std::string& name,
                         const glm::vec4& value) override;
  virtual void SetMat4(const std::string& name,
                       const glm::mat4& value) override;
  const std::string& GetName() const override;

private:
  void CheckCompileErrors(unsigned int shader, const std::string& type);

private:
  uint32_t m_ShaderID_ = 0;
};

} // namespace ge
