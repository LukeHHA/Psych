#pragma once

#include "AssetHandle.h"
#include "AssetTypes.h"

namespace psych
{
class Asset
{
public:
  Asset()            = default;
  virtual ~Asset()   = default;

  AssetHandle handle = AssetHandle::Nill();

  static AssetType GetStaticType() { return AssetType::None; }
  virtual AssetType GetAssetType() const { return AssetType::None; }

  friend bool operator==(Asset const& lhs, Asset const& rhs) noexcept { return lhs.handle == rhs.handle; }

private:
};
} // namespace psych
