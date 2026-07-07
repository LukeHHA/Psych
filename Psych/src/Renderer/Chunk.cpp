#include "Chunk.h"
#include "Renderer/Buffer.h"

namespace psych
{
void ChunkManager::Init(const ChunkManagerSpecification& specs)
{
  m_Specs_ = specs;

  for (uint32_t y = 0; y < m_Specs_.VisibaleChunckDistance; y++) {
    for (uint32_t x = 0; x < m_Specs_.VisibaleChunckDistance; x++) {
      GenChunk();
    }
  }
}

void ChunkManager::GenChunk()
{
  for (uint32_t y = 0; y < m_Specs_.ChunkHeight; y++) {
    for (uint32_t x = 0; x < m_Specs_.ChunkWidth; x++) {
      for (uint32_t z = 0; z < m_Specs_.ChunkDepth; z++) {
      }
    }
  }
}
} // namespace psych
