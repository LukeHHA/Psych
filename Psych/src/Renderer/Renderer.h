 
/**************************************************************************/
/*  Renderer.h                                                            */                                                            
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PSYCH ENGINE                               */
/**************************************************************************/
/* Copyright (c)  Luke Howe                                               */                                                  
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "Core/Core.h"
#include "Renderer/Mesh.h"
#include "Renderer/RenderTarget.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/RendererCommandBuffer.h"
#include "Renderer/RendererContext.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexTypes.h"
#include <glm/vec3.hpp>

namespace psych
{
struct PsychEngineSpecification;

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

  static Expected<void, errors::RendererError> Init(const PsychEngineSpecification& specs);
  static void Shutdown();
  static void SetClearColour(const glm::vec3& color);
  static void Clear();
  static RendererAPIType GetCurrentAPI()
  {
    CORE_ASSERT(s_Initialized_ && s_RendererAPI_, "Renderer API is not initialized")
    return RendererAPI::Current();
  }
  static void SetRendererAPI(RendererAPIType type);
  static void BeginScene(RenderTarget& target);
  static void EndScene();
  static void SubmitMesh(Shared<Mesh> mesh);
  static void SubmitStaticMesh(Shared<Mesh> mesh);

private:
  static void Submit(auto&& func)
  {
    CORE_ASSERT(s_Initialized_ && s_RendererCommandBuffer_, "Renderer command buffer is not initialized")
    s_RendererCommandBuffer_->Push(std::forward<decltype(func)>(func));
  }

  static void Flush();

private:
  inline static Unique<RendererAPI> s_RendererAPI_                     = nullptr;
  inline static Unique<RendererContext> s_RendererContext_             = nullptr;
  inline static Unique<RendererCommandBuffer> s_RendererCommandBuffer_ = nullptr;
  inline static RenderTarget* s_ActiveRenderTarget_                    = nullptr;
  inline static bool s_Initialized_                                    = false;
  inline static RenderData s_Data;
};
} // namespace psych
