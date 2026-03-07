#pragma once

#include "Core/Base.h"
#include "Core/GameEngine.h"
#include "Renderer/Mesh.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/RendererCommandBuffer.h"
#include "Renderer/RendererContext.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexTypes.h"
#include <glm/vec3.hpp>

namespace ge
{
struct MeshData {
};

struct StaticMeshData {
};

struct RenderData {
  uint32_t drawCalls = 0;
  MeshData meshData;
  StaticMeshData staticMeshData;
};

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
  static void SetRendererAPI(RendererAPIType type);
  static void BeginScene();
  static void EndScene();
  static void SubmitMesh(Shared<Mesh> mesh);
  static void SubmitStaticMesh(Shared<Mesh> mesh);

private:
  static void Submit(auto&& func)
  {
    s_RendererCommandBuffer_->Push(std::forward<decltype(func)>(func));
  }

  static void Flush();

private:
  inline static Unique<RendererAPI> s_RendererAPI_         = nullptr;
  inline static Unique<RendererContext> s_RendererContext_ = nullptr;
  inline static Unique<RendererCommandBuffer> s_RendererCommandBuffer_ =
      nullptr;
  inline static bool s_Initialized_ = false;
  inline static RenderData s_Data;
};
} // namespace ge
