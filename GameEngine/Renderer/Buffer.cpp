#include "Buffer.h"
#include "Core/Base.h"
#include "Renderer/Platform/Opengl/OpenglBuffer.h"

namespace ge
{
Unique<VertexBuffer> VertexBuffer::Create(float& vertices)
{
  return CreateUnique<OpenglVertexBuffer>(vertices);
}

Unique<IndexBuffer> IndexBuffer::Create(float& vertices)
{
  return CreateUnique<OpenglIndexBuffer>(vertices);
}
} // namespace ge
