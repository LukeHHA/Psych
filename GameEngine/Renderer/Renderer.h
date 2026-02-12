#pragma once

#include "Core/Base.h"
#include "Core/GameEngine.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/RendererContext.h"
#include <glm/vec3.hpp>

namespace ge
{
class Renderer
{
public:
  Renderer()  = delete;
  ~Renderer() = delete;

  static void Init(GameEngineSpecification& specs);
  static void Shutdown();
  static void SetClearColour(const glm::vec3& color);
  static void Clear();
  static RendererAPIType GetCurrentAPI() { return s_RendererAPI_->Current(); }
  static void SetRendererAPI(GameEngineSpecification& specs);

private:
  inline static Unique<RendererAPI> s_RendererAPI_ = nullptr;
  inline static bool s_Initialized_                = false;
  static Unique<RendererContext> s_RendererContext_;
};
} // namespace ge
