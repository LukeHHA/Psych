#pragma once
#include "Asset/AssetExtensions.h"
#include "Asset/AssetMetaData.h"
#include "ge_expected"

namespace ge
{

class IAssetLibrary
{
public:
  IAssetLibrary()          = default;
  virtual ~IAssetLibrary() = default;

  virtual Expected<void, std::string>
  ProcessDirectory(const util::FilePath& path)                             = 0;
  virtual Expected<void, std::string> AddAsset(const util::FilePath& path) = 0;
};

class AssetLibrary : public IAssetLibrary
{
public:
  AssetLibrary()          = default;
  virtual ~AssetLibrary() = default;

  virtual Expected<void, std::string>
  ProcessDirectory(const util::FilePath& path) override
  {
    for (const auto& dirEntry : util::recursive_directory_iterator(path)) {
      if (!dirEntry.is_regular_file())
        continue;
      if (!dirEntry.path().has_extension())
        continue;

      AddAsset(dirEntry);
    }
    return {};
  }

  virtual Expected<void, std::string>
  AddAsset(const util::FilePath& asset_path) override
  {
    auto ext = asset_path.extension().string();

    if (auto it = s_AssetExtensionMap.find(ext);
        it != s_AssetExtensionMap.end()) {

      AssetMetaData metadata;
      metadata.handle    = AssetHandle{};
      metadata.type      = it->second;
      metadata.path      = asset_path;
      metadata.is_loaded = true;

      m_AssetLibrary_.emplace(metadata.handle, metadata);
    }
    return {};
  }

private:
  std::unordered_map<AssetHandle, AssetMetaData> m_AssetLibrary_;
};
} // namespace ge
