 
/**************************************************************************/
/*  RendererContext.h                                                     */                                                            
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
#include "Renderer/RendererAPI.h"
#include "expected.h"

struct GLFWwindow;
namespace psych
{

class RendererContext
{
public:
  RendererContext()                                           = default;
  RendererContext(const RendererContext&)                     = default;
  RendererContext(RendererContext&&)                          = delete;
  RendererContext& operator=(const RendererContext&)          = default;
  RendererContext& operator=(RendererContext&&)               = delete;
  virtual ~RendererContext()                                  = default;

  virtual Expected<void, errors::RendererError> Init()        = 0;
  virtual void SwapBuffers()                                  = 0;
  [[nodiscard]] virtual RendererAPIType GetCurrentAPI() const = 0;

  static Expected<Unique<RendererContext>, errors::RendererError> Create(GLFWwindow* window);
};

class RendererContextHeadless : public RendererContext
{
public:
  RendererContextHeadless(const RendererContextHeadless&)            = default;
  RendererContextHeadless(RendererContextHeadless&&)                 = delete;
  RendererContextHeadless& operator=(const RendererContextHeadless&) = default;
  RendererContextHeadless& operator=(RendererContextHeadless&&)      = delete;
  RendererContextHeadless(GLFWwindow* window);
  ~RendererContextHeadless() override = default;

  Expected<void, errors::RendererError> Init() override;
  void SwapBuffers() override;
  [[nodiscard]] RendererAPIType GetCurrentAPI() const override { return RendererAPIType::TEST_HEADLESS; }

private:
  GLFWwindow* m_WindowHandle;
};
} // namespace psych
