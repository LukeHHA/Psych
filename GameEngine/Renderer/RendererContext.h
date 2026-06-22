#pragma once

#include "Core/Core.h"
#include "Errors/Errors.h"
#include "Renderer/RendererAPI.h"
#include "ge_expected"

struct GLFWwindow;
namespace ge
{

class RendererContext
{
public:
  RendererContext()                                    = default;
  virtual ~RendererContext()                           = default;

  virtual Expected<void, errors::RendererError> Init() = 0;
  virtual void SwapBuffers()                           = 0;
  virtual RendererAPIType GetCurrentAPI() const        = 0;

  static Expected<Shared<RendererContext>, errors::RendererError> Create(GLFWwindow* window);
};

class RendererContextHeadless : public RendererContext
{
public:
  RendererContextHeadless(GLFWwindow* window);
  virtual ~RendererContextHeadless() = default;

  virtual Expected<void, errors::RendererError> Init() override;
  void SwapBuffers() override;
  RendererAPIType GetCurrentAPI() const override { return RendererAPIType::TEST_HEADLESS; }

private:
  GLFWwindow* m_WindowHandle;
};
} // namespace ge
