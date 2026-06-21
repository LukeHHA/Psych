#include "AssetManager.h"

namespace ge
{
void AssetManager::Init(const AssetManagerSpec& spec)
{
  m_AssetLibrary_.ProcessDirectory(m_AssetPath_);
}

void AssetManager::Shutdown() {}
} // namespace ge
