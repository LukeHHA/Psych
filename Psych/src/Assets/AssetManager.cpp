#include "AssetManager.h"

namespace psych
{
void AssetManager::Init(const AssetManagerSpec& spec) { m_AssetLibrary_.ProcessDirectory(m_AssetPath_); }

void AssetManager::Shutdown() {}
} // namespace psych
