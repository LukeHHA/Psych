#include "Renderer.h"

namespace ge
{
Renderer::Renderer() { m_RendererAPI_ = RendererAPI::Create(); }

void Renderer::SetClearColour(glm::vec3& colour)
{
  m_RendererAPI_->SetClearColour(colour);
}
} // namespace ge
