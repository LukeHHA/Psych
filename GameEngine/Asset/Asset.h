#pragma once

#include "Core/UUID.h"
namespace ge
{
using AssetHandle = UUID;

enum class AssetType { None, Texture2D, Mesh, StaticMesh };

class Asset
{
public:
  Asset()            = default;
  virtual ~Asset()   = default;

  AssetHandle handle = UUID::Nill();

  static AssetType GetStaticType() { return AssetType::None; }
  virtual AssetType GetAssetType() const { return AssetType::None; }

  friend bool operator==(Asset const& lhs, Asset const& rhs) noexcept
  {
    return lhs.handle == rhs.handle;
  }

private:
};
} // namespace ge
