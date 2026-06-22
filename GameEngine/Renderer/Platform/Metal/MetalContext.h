#pragma once

#include "Renderer/RendererAPI.h"
#include "Renderer/RendererContext.h"

namespace ge
{
class MetalContext : public RendererContext
{
public:
  MetalContext();
  // MetalContext(GLFWwindow* window);
  virtual ~MetalContext() = default;

  virtual Expected<void, errors::RendererError> Init() override;
  virtual void SwapBuffers() override;
  virtual RendererAPIType GetCurrentAPI() const override { return RendererAPIType::METAL; }

private:
  // GLFWwindow* m_WindowHandle;
  // RendererAPIType m_API = RendererAPIType::METAL;
};
} // namespace ge
