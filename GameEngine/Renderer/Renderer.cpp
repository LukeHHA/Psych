#include "Renderer.h"
#include "Core/GameEngine.h"
#include "Debug/Assert.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/VertexTypes.h"
#include <iostream>

namespace ge
{
Expected<void, errors::RendererError> Renderer::Init(GameEngineSpecification& specs)
{
  if (s_Initialized_ || s_RendererAPI_ != nullptr) {
    return Unexpected(errors::RendererError::AlreadyInitialized);
  }

  RendererAPI::SetAPI(specs.RenderingAPI);
  auto rendererAPI = RendererAPI::Create();
  if (!rendererAPI) {
    RendererAPI::SetAPI(RendererAPIType::NONE);
    return Unexpected(rendererAPI.error());
  }

  s_RendererAPI_ = std::move(rendererAPI.value());
  if (!s_RendererAPI_) {
    RendererAPI::SetAPI(RendererAPIType::NONE);
    return Unexpected(errors::RendererError::APICreationFailed);
  }

  auto initResult = s_RendererAPI_->Init();
  if (!initResult) {
    s_RendererAPI_.reset();
    RendererAPI::SetAPI(RendererAPIType::NONE);
    return Unexpected(initResult.error());
  }

  s_RendererCommandBuffer_ = CreateUnique<RendererCommandBuffer>();
  s_Initialized_           = true;
  return {};
}

void Renderer::Shutdown()
{
  if (!s_Initialized_) {
    return;
  }

  s_RendererCommandBuffer_.reset();
  s_RendererAPI_.reset();
  s_ActiveRenderTarget_ = nullptr;
  s_Data                = {};
  s_Initialized_        = false;

  RendererAPI::SetAPI(RendererAPIType::NONE);
}

void Renderer::BeginScene(RenderTarget& target)
{
  CORE_ASSERT(s_Initialized_ && s_RendererCommandBuffer_, "Renderer command buffer is not initialized")
  CORE_ASSERT(s_RendererAPI_, "Renderer API is not initialized")
  CORE_ASSERT(s_ActiveRenderTarget_ == nullptr, "Renderer::BeginScene called while another scene is active")
  s_ActiveRenderTarget_ = &target;
  s_ActiveRenderTarget_->Bind();
  s_RendererAPI_->SetViewPort(0, 0, s_ActiveRenderTarget_->GetWidth(), s_ActiveRenderTarget_->GetHeight());
  Clear();
  s_RendererCommandBuffer_->Begin();
}

void Renderer::EndScene()
{
  CORE_ASSERT(s_Initialized_ && s_RendererCommandBuffer_, "Renderer command buffer is not initialized")
  CORE_ASSERT(s_ActiveRenderTarget_, "Renderer::EndScene called without an active scene")
  s_RendererCommandBuffer_->End();
  Flush();
  s_ActiveRenderTarget_->Unbind();
  s_ActiveRenderTarget_ = nullptr;
}

void Renderer::SubmitMesh(Shared<Mesh> mesh)
{
  Submit([mesh]() {

  });
}

void Renderer::Flush() { s_Data.drawCalls = 0; }

void Renderer::SetRendererAPI(RendererAPIType type) { RendererAPI::SetAPI(type); }

void Renderer::SetClearColour(const glm::vec3& colour)
{
  CORE_ASSERT(s_Initialized_ && s_RendererAPI_, "Renderer API is not initialized")
  s_RendererAPI_->SetClearColour(colour);
}

void Renderer::Clear()
{
  CORE_ASSERT(s_Initialized_ && s_RendererAPI_, "Renderer API is not initialized")
  s_RendererAPI_->Clear();
}
} // namespace ge
