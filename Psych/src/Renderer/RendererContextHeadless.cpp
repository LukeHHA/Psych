 
/**************************************************************************/
/*  RendererContextHeadless.cpp                                           */                                                            
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

#include "Core/glad_glfw_incl.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "RendererContext.h"
#include "glad/glad.h"
namespace psych
{
RendererContextHeadless::RendererContextHeadless(GLFWwindow* window) : m_WindowHandle(window) { CORE_PROFILE_FUNCTION(); }

Expected<void, errors::RendererError> RendererContextHeadless::Init()
{
  CORE_PROFILE_FUNCTION();
  if (m_WindowHandle == nullptr) {
    return Unexpected(errors::RendererError::ContextCreationFailed);
  }

  glfwMakeContextCurrent(m_WindowHandle);
  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (!status) {
    return Unexpected(errors::RendererError::ContextInitializationFailed);
  }

  CORE_LOG_INFO("OpenGL Info:");
  CORE_LOG_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
  CORE_LOG_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
  CORE_LOG_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
  CORE_LOG_INFO("OpenGL Context Initialized");
  GLint major = 0, minor = 0;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  CORE_LOG_INFO("Opengl major version: {0}", major);
  CORE_LOG_INFO("Opengl minor version {0}", minor);
  return {};
}
void RendererContextHeadless::SwapBuffers()
{
  CORE_PROFILE_FUNCTION();
  glfwSwapBuffers(m_WindowHandle);
}

} // namespace psych
