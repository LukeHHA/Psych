#pragma once

#include "Core/Base.h"
#include "Renderer/RendererAPI.h"

struct GLFWwindow;
namespace ge
{

class RendererContext
{
public:
  RendererContext()                             = default;
  virtual ~RendererContext()                    = default;

  virtual void Init()                           = 0;
  virtual void SwapBuffers()                    = 0;
  virtual RendererAPIType GetCurrentAPI() const = 0;

  static Shared<RendererContext> Create(GLFWwindow* window);
};

class RendererContextHeadless : public RendererContext
{
public:
  RendererContextHeadless(GLFWwindow* window);
  virtual ~RendererContextHeadless() = default;

  virtual void Init();
  virtual void SwapBuffers();
  virtual RendererAPIType GetCurrentAPI() const
  {
    return RendererAPIType::TEST_HEADLESS;
  }

private:
  GLFWwindow* m_WindowHandle;
};
} // namespace ge
