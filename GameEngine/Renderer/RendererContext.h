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
} // namespace ge
