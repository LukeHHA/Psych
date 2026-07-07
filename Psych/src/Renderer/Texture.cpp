#include "Texture.h"
#include "Renderer/RendererAPI.h"

namespace psych
{
Shared<Texture> Texture2D::Create(const std::string& path)
{
  switch (RendererAPI::Current()) {
  case psych::RendererAPIType::TEST_HEADLESS:
  case psych::RendererAPIType::OPENGL:
    return CreateShared<OpenglTexture2D>(path);
  default:
    CORE_ASSERT(false, "Unsupported API type is selected when trying to create texture");
  }
}
} // namespace psych
