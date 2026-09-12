 
/**************************************************************************/
/*  RendererContext.cpp                                                   */                                                            
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

#include "Renderer/RendererContext.h"
#include "Renderer/Platform/Opengl/OpenglContext.h"
#include "Renderer/Platform/Vulkan/VulkanContext.h"
#include "Renderer/RendererAPI.h"
#include <Debug/Instrumentor.h>

namespace psych
{
Expected<Unique<RendererContext>, errors::RendererError> RendererContext::Create(GLFWwindow* window)
{
  CORE_PROFILE_FUNCTION();

  if (window == nullptr) {
    return Unexpected(errors::RendererError::ContextCreationFailed);
  }

  switch (RendererAPI::Current()) {
  case RendererAPIType::NONE:
    return Unexpected(errors::RendererError::UnsupportedAPI);
  case RendererAPIType::TEST_HEADLESS:
    return CreateUnique<RendererContextHeadless>(window);
  case RendererAPIType::OPENGL:
    return CreateUnique<OpenglContext>(window);
  case RendererAPIType::VULKAN:
    return CreateUnique<VulkanContext>();
  case RendererAPIType::METAL:
    return CreateUnique<OpenglContext>(window);
  default:
    return Unexpected(errors::RendererError::UnsupportedAPI);
  }
}
} // namespace psych
