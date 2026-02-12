#include "Renderer.h"
#include "Core/GameEngine.h"
#include "Renderer/RendererAPI.h"
#include <iostream>

namespace ge
{
void Renderer::Init(GameEngineSpecification& specs)
{
  CORE_ASSERT(!s_Initialized_, "Renderer has already been initialized")
  CORE_ASSERT(s_RendererAPI_ == nullptr,
              "Currently only one API can be used per context")
  SetRendererAPI(specs);
  s_RendererAPI_ = RendererAPI::Create();
  CORE_ASSERT(s_RendererAPI_ != nullptr,
              "Creation of the render API failed and returned nullptr")
  s_Initialized_ = true;
}

void Renderer::SetRendererAPI(GameEngineSpecification& specs)
{
  std::cout << static_cast<int>(specs.RenderingAPI) << std::endl;
  switch (specs.RenderingAPI) {
  case RendererAPIType::OPENGL:
    s_RendererAPI_->SetAPI(RendererAPIType::OPENGL);
    return;
  }
  CORE_ASSERT(false, "Currently Opengl is the only supported API")
}

void Renderer::SetClearColour(const glm::vec3& colour)
{
  s_RendererAPI_->SetClearColour(colour);
}

void Renderer::Clear() { s_RendererAPI_->Clear(); }
} // namespace ge
