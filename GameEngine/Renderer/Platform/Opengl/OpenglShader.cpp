#include "OpenglShader.h"
#include "FileSystem/FileSystem.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

namespace ge
{
OpenglShader::OpenglShader(const std::string& vertexPath,
                           const std::string& fragPath, const std::string& name)
{
  const std::string vertexSrc = util::Filesystem::StreamFile(vertexPath);
  const std::string fragSrc   = util::Filesystem::StreamFile(fragPath);

  const char* vertexCode      = vertexSrc.c_str();
  const char* fragCode        = fragSrc.c_str();

  unsigned int vertex, fragment;
  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexCode, NULL);
  glCompileShader(vertex);
  CheckCompileErrors(vertex, "VERTEX");
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragCode, NULL);
  glCompileShader(fragment);
  CheckCompileErrors(fragment, "FRAGMENT");
  // shader Program
  m_ShaderID_ = glCreateProgram();
  glAttachShader(m_ShaderID_, vertex);
  glAttachShader(m_ShaderID_, fragment);
  glLinkProgram(m_ShaderID_);
  CheckCompileErrors(m_ShaderID_, "PROGRAM");
  // delete the shaders as they're linked into our program now and no longer
  // necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void OpenglShader::Bind() const { glUseProgram(m_ShaderID_); }
void OpenglShader::Unbind() const { glUseProgram(0); }
void OpenglShader::SetInt(const std::string& name, int value)
{
  GLint location = glGetUniformLocation(m_ShaderID_, name.c_str());
  glUniform1i(location, value);
}

void OpenglShader::SetIntArray(const std::string& name, int values[],
                               uint32_t count)
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

void OpenglShader::CheckCompileErrors(unsigned int shader,
                                      const std::string& type)
{
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout
          << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout
          << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
    }
  }
}
} // namespace ge
