 
/**************************************************************************/
/*  Chunk.h                                                               */                                                            
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
#include "Renderer/VertexTypes.h"
#include "glm/mat4x4.hpp"
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace psych
{

#define MAX_CHUNK_SIZE 256 * 256 * 256

class VertexBuffer;
class IndexBuffer;
class VertexArray;

using ChunkID   = uint32_t;
using TextureID = uint32_t;

struct ChunkData {
  std::array<TextureID, MAX_CHUNK_SIZE> blocks;
  std::array<uint8_t, MAX_CHUNK_SIZE> light;
};

struct ChunkMesh {
  std::vector<VertexPNT> opaqueVertices;
  std::vector<Index> opaqueIndices;

  std::vector<VertexPNT> transparentVertices;
  std::vector<Index> transparentIndices;

  bool valid = false;
};

struct ChunkRenderProxy {
  Shared<VertexBuffer> vertexBuffer;
  Shared<IndexBuffer> indexBuffer;
  Shared<VertexArray> vertexArray;
};

struct ChunkRecord {
  ChunkID id;
  glm::uvec3 coord;
  ChunkData data;
  ChunkMesh mesh;
  uint32_t dirtyFlags = 0;
};

struct ChunkManagerSpecification {
  ChunkManagerSpecification()     = default;
  uint32_t ChunkHeight            = 256;
  uint32_t ChunkWidth             = 256;
  uint32_t ChunkDepth             = 256;
  uint32_t VisibaleChunckDistance = 12;
  uint32_t seed;
};

class ChunkManager
{
public:
  void Init(const ChunkManagerSpecification& specs = ChunkManagerSpecification());
  void Shutdown();
  void UpdateVisibleChunkDistance(const int& distance);
  void RegenerateChunks(const uint32_t& seed);
  void UpdateSeed(const uint32_t& seed);
  size_t GetBlock(const int& x, const int& y, const int& z) const;
  size_t GetBlock(const glm::vec3& pos) const;
  ChunkID AddChunk();

private:
  void GenChunk();

private:
  ChunkManager();
  ~ChunkManager();
  ChunkRenderProxy m_ChunkRenderProxy_;
  ChunkManagerSpecification m_Specs_;
  std::vector<ChunkRecord> m_Chunks;
  std::unordered_map<ChunkID, size_t> m_IdToIndex;
  // TextureLibrary m_TextureLibrary_;
};
} // namespace psych
