#include "Renderer.h"
#include "Core/GameEngine.h"
#include "Debug/Assert.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/VertexTypes.h"
#include <iostream>

namespace ge
{
void Renderer::Init(GameEngineSpecification& specs)
{
  CORE_ASSERT(!s_Initialized_, "Renderer has already been initialized")
  CORE_ASSERT(s_RendererAPI_ == nullptr,
              "Currently only one API can be used per context")
  SetRendererAPI(specs.RenderingAPI);
  s_RendererAPI_ = RendererAPI::Create();
  CORE_ASSERT(s_RendererAPI_ != nullptr,
              "Creation of the render API failed and returned nullptr")
  s_Initialized_ = true;
}

void Renderer::BeginScene() { s_RendererCommandBuffer_->Begin(); }

void Renderer::EndScene()
{
  s_RendererCommandBuffer_->End();
  Flush();
}

void Renderer::SubmitMesh(Shared<Mesh> mesh)
{
  Submit([mesh]() {

  });
}

void Renderer::Flush() { s_Data.drawCalls = 0; }

void Renderer::SetRendererAPI(RendererAPIType type)
{
  switch (type) {
  case ge::RendererAPIType::TEST_HEADLESS:
    s_RendererAPI_->SetAPI(RendererAPIType::TEST_HEADLESS);
    return;
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
