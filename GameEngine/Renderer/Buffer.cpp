#include "Buffer.h"
#include "Core/Core.h"
#include "Renderer/Platform/Opengl/OpenglBuffer.h"
#include <cstdint>

namespace ge
{
Shared<VertexBuffer> VertexBuffer::Create(const float* vertices, uint32_t count)
{
  return CreateShared<OpenglVertexBuffer>(vertices, count);
}

Shared<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
  return CreateShared<OpenglVertexBuffer>(size);
}

Shared<IndexBuffer> IndexBuffer::Create(const uint32_t* indices,
                                        const uint32_t count)
{
  return CreateShared<OpenglIndexBuffer>(indices, count);
}
} // namespace ge
