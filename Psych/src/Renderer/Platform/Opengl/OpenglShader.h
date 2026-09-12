 
/**************************************************************************/
/*  OpenglShader.h                                                        */                                                            
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

#include "../../Shader.h"

namespace psych
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

} // namespace psych
