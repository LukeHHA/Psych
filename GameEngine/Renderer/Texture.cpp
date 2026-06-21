#include "Texture.h"
#include "Renderer/RendererAPI.h"

namespace ge
{
Shared<Texture> Texture2D::Create(const std::string& path)
{
  switch (RendererAPI::Current()) {
  case ge::RendererAPIType::TEST_HEADLESS:
  case ge::RendererAPIType::OPENGL:
    return CreateShared<OpenglTexture2D>(path);
  default:
    CORE_ASSERT(
        false,
        "Unsupported API type is selected when trying to create texture");
  }
}
} // namespace ge
