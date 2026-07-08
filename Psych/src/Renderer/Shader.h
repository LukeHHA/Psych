 
/**************************************************************************/
/*  Shader.h                                                              */                                                            
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

#include "Core/Core.h"
#include "Errors/Errors.h"
#include "expected.h"
#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

namespace psych
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

  static Expected<Shared<Shader>, errors::ShaderError> Create(const std::string& vertexSrc, const std::string& fragSrc, const std::string& name);
};

class ShaderLibrary
{
public:
  ShaderLibrary()  = default;
  ~ShaderLibrary() = default;
  void Add(const Shared<Shader>& shader, const std::string& name);
  Expected<Shared<Shader>, errors::ShaderError> Load(const std::string& vertexSrc, const std::string& fragSrc, const std::string& name);
  Expected<Shared<Shader>, errors::ShaderLibraryError> Get(const std::string& name) const;
  bool Exists(const std::string& name) const;

  static Shared<ShaderLibrary> Create();

private:
  std::unordered_map<std::string, Shared<Shader>> m_Shaders_;
};
} // namespace psych
