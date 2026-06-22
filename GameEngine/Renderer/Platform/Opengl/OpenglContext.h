#pragma once

#include "Renderer/RendererAPI.h"
#include "Renderer/RendererContext.h"

namespace ge
{
class OpenglContext : public RendererContext
{
public:
  OpenglContext(GLFWwindow* window);
  virtual ~OpenglContext() = default;

  virtual Expected<void, errors::RendererError> Init() override;
  virtual void SwapBuffers() override;
  virtual RendererAPIType GetCurrentAPI() const override { return RendererAPIType::OPENGL; }

private:
  GLFWwindow* m_WindowHandle;
};
} // namespace ge
