 
/**************************************************************************/
/*  Chunk.cpp                                                             */                                                            
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
