#pragma once

#include "Renderer/RendererAPI.h"
#include "Renderer/RendererContext.h"

namespace ge
{
class VulkanContext : public RendererContext
{
public:
  VulkanContext();
  // VulkanContext(GLFWwindow* window);
  virtual ~VulkanContext() = default;

  virtual Expected<void, errors::RendererError> Init() override;
  virtual void SwapBuffers() override;
  virtual RendererAPIType GetCurrentAPI() const override { return RendererAPIType::VULKAN; }

private:
  // GLFWwindow* m_WindowHandle;
  // RendererAPIType m_API = RendererAPIType::VULKAN;
};
} // namespace ge
