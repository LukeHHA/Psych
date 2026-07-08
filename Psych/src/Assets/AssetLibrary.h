 
/**************************************************************************/
/*  AssetLibrary.h                                                        */                                                            
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PSYCH ENGINE                               */
/**************************************************************************/
/* Copyright (c)  Luke Howe                                               */                                                  
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once
#include "Asset/AssetExtensions.h"
#include "Asset/AssetMetaData.h"
#include "expected.h"

namespace psych
{

class IAssetLibrary
{
public:
  IAssetLibrary()                                                                  = default;
  virtual ~IAssetLibrary()                                                         = default;

  virtual Expected<void, std::string> ProcessDirectory(const util::FilePath& path) = 0;
  virtual Expected<void, std::string> AddAsset(const util::FilePath& path)         = 0;
};

class AssetLibrary : public IAssetLibrary
{
public:
  AssetLibrary()          = default;
  virtual ~AssetLibrary() = default;

  virtual Expected<void, std::string> ProcessDirectory(const util::FilePath& path) override
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

  virtual Expected<void, std::string> AddAsset(const util::FilePath& asset_path) override
  {
    auto ext = asset_path.extension().string();

    if (auto it = s_AssetExtensionMap.find(ext); it != s_AssetExtensionMap.end()) {

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
} // namespace psych
