#pragma once

#include "Asset/AssetLibrary.h"
namespace ge
{
struct AssetManagerSpec {
  util::FilePath asset_path;
};

class AssetLibrary;
class AssetManager
{
public:
  static void Init(const AssetManagerSpec& spec);
  static void Shutdown();

private:
  AssetManager()  = default;
  ~AssetManager() = default;

private:
  AssetLibrary m_AssetLibrary_;
  util::FilePath m_AssetPath_ = "/assets/";
};
} // namespace ge
