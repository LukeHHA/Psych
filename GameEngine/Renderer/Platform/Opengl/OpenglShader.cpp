#include "OpenglShader.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

namespace ge
{
OpenglShader::OpenglShader(const std::string& name) : m_ShaderName_(name) {}

OpenglShader::~OpenglShader()
{
  if (m_ShaderID_ != 0) {
    glDeleteProgram(m_ShaderID_);
  }
}

Expected<Shared<Shader>, errors::ShaderError> OpenglShader::Create(const std::string& vertexSrc, const std::string& fragSrc, const std::string& name)
{
  auto shader        = CreateShared<OpenglShader>(name);
  const auto compile = shader->Compile(vertexSrc, fragSrc);
  if (!compile) {
    return Unexpected(compile.error());
  }

  return shader;
}

Expected<void, errors::ShaderError> OpenglShader::Compile(const std::string& vertexSrc, const std::string& fragSrc)
{
  if (glfwGetCurrentContext() == nullptr) {
    return Unexpected(errors::ShaderError::NoGraphicsContext);
  }

  const char* vertexCode = vertexSrc.c_str();
  const char* fragCode   = fragSrc.c_str();

  unsigned int vertex, fragment;
  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexCode, NULL);
  glCompileShader(vertex);
  auto vertexResult = CheckCompileErrors(vertex, "VERTEX");
  if (!vertexResult) {
    glDeleteShader(vertex);
    return Unexpected(vertexResult.error());
  }

  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragCode, NULL);
  glCompileShader(fragment);
  auto fragmentResult = CheckCompileErrors(fragment, "FRAGMENT");
  if (!fragmentResult) {
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return Unexpected(fragmentResult.error());
  }

  // shader Program
  m_ShaderID_ = glCreateProgram();
  glAttachShader(m_ShaderID_, vertex);
  glAttachShader(m_ShaderID_, fragment);
  glLinkProgram(m_ShaderID_);
  auto linkResult = CheckCompileErrors(m_ShaderID_, "PROGRAM");
  if (!linkResult) {
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteProgram(m_ShaderID_);
    m_ShaderID_ = 0;
    return Unexpected(linkResult.error());
  }

  // delete the shaders as they're linked into our program now and no longer
  // necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  return {};
}

void OpenglShader::Bind() const { glUseProgram(m_ShaderID_); }
void OpenglShader::Unbind() const { glUseProgram(0); }
void OpenglShader::SetInt(const std::string& name, int value)
{
  GLint location = glGetUniformLocation(m_ShaderID_, name.c_str());
  glUniform1i(location, value);
}

void OpenglShader::SetIntArray(const std::string& name, int values[], uint32_t count)
{
  GLint location = glGetUniformLocation(m_ShaderID_, name.c_str());
  glUniform1iv(location, count, values);
}
void OpenglShader::SetFloat(const std::string& name, float value)
{
  GLint location = glGetUniformLocation(m_ShaderID_, name.c_str());
  glUniform1f(location, value);
}
void OpenglShader::SetFloat2(const std::string& name, const glm::vec2& value)
{
  GLint location = glGetUniformLocation(m_ShaderID_, name.c_str());
  glUniform2f(location, value.x, value.y);
}
void OpenglShader::SetFloat3(const std::string& name, const glm::vec3& value)
{
  GLint location = glGetUniformLocation(m_ShaderID_, name.c_str());
  glUniform3f(location, value.x, value.y, value.z);
}
void OpenglShader::SetFloat4(const std::string& name, const glm::vec4& value)
{
  GLint location = glGetUniformLocation(m_ShaderID_, name.c_str());
  glUniform4f(location, value.x, value.y, value.z, value.w);
}
void OpenglShader::SetMat4(const std::string& name, const glm::mat4& value)
{
  GLint location = glGetUniformLocation(m_ShaderID_, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

Expected<void, errors::ShaderError> OpenglShader::CheckCompileErrors(unsigned int shader, const std::string& type)
{
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << "Shader Name: " << m_ShaderName_ << '\n'
                << std::endl;
      return Unexpected(errors::ShaderError::CompileFailed);
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << "Shader Name: " << m_ShaderName_ << '\n'
                << std::endl;
      return Unexpected(errors::ShaderError::LinkFailed);
    }
  }

  return {};
}
} // namespace ge
