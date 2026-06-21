#pragma once

#include "Asset/Asset.h"
#include "FileSystem/FileSystem.h"
namespace ge
{
struct AssetMetaData {
  AssetHandle handle = AssetHandle::Nill();
  util::FilePath path;
  AssetType type;
  std::atomic<bool> is_loaded = false;
};

} // namespace ge
