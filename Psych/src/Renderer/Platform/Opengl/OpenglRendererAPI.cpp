 
/**************************************************************************/
/*  OpenglRendererAPI.cpp                                                 */                                                            
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

#include "OpenglRendererAPI.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Instrumentor.h"
#include "Logging/Logging.h"
#include "glad/glad.h"

namespace psych
{

void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam)
{
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    CORE_LOG_CRITICAL(message);
    return;
  case GL_DEBUG_SEVERITY_MEDIUM:
    CORE_LOG_ERROR(message);
    return;
  case GL_DEBUG_SEVERITY_LOW:
    CORE_LOG_WARN(message);
    return;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    CORE_LOG_TRACE(message);
    return;
  }

  CORE_ASSERT(false, "Unknown severity level!");
}

Expected<void, errors::RendererError> OpenglRendererAPI::Init()
{
  CORE_PROFILE_FUNCTION();

  if (glfwGetCurrentContext() == nullptr) {
    return Unexpected(errors::RendererError::ContextInitializationFailed);
  }

  const bool debugOutputAvailable = (GLAD_GL_VERSION_4_3 || GLAD_GL_KHR_debug) && glad_glDebugMessageCallback != nullptr && glad_glDebugMessageControl != nullptr;

  if (debugOutputAvailable) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
  } else {
    CORE_LOG_WARN("OpenGL debug output is not available for this context");
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  return {};
}
void OpenglRendererAPI::SetClearColour(const glm::vec3& colour) { glClearColor(colour.x, colour.y, colour.z, 0.01f); }
void OpenglRendererAPI::SetViewPort(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height) { glViewport(x, y, width, height); }
void OpenglRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenglRendererAPI::DrawIndexed(const Shared<VertexArray>& vertexArray, const uint32_t indexCount)
{
  CORE_PROFILE_FUNCTION();
  vertexArray->Bind();
  uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetIndexCount();
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
} // namespace psych
