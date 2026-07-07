#pragma once

#include "Asset/Asset.h"
#include "FileSystem/FileSystem.h"
namespace psych
{
struct AssetMetaData {
  AssetHandle handle = AssetHandle::Nill();
  util::FilePath path;
  AssetType type;
  std::atomic<bool> is_loaded = false;
};

} // namespace psych
