 
/**************************************************************************/
/*  Window.cpp                                                            */                                                            
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

#include "Window.h"
#include "Debug/Assert.h"
#include "Debug/Instrumentor.h"
#include "Logging/Logging.h"
#include "Window/EngineWindow.h"
#include "Window/HeadlessWindow.h"

namespace psych
{
class EventHandler;
Expected<Unique<Window>, errors::WindowError> Window::Create(const std::string& title, unsigned int width, unsigned int height, EventHandler& eventHandler)
{
  CORE_PROFILE_FUNCTION();
  Unique<Window> window;

  switch (RendererAPI::Current()) {
  case RendererAPIType::TEST_HEADLESS:
    window = CreateUnique<HeadlessWindow>();
    break;

  case RendererAPIType::OPENGL:
  case RendererAPIType::VULKAN:
  case RendererAPIType::METAL:
    window = CreateUnique<EngineWindow>();
    break;
  case RendererAPIType::NONE:
    return Unexpected(errors::WindowError::InitializationFailed);
  }

  if (!window) {
    return Unexpected(errors::WindowError::InitializationFailed);
  }

  const auto result = window->Init(title, width, height, eventHandler);
  if (!result) {
    return Unexpected(result.error());
  }

  return window;
}
} // namespace psych
