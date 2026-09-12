 
/**************************************************************************/
/*  AssetTypes.h                                                          */                                                            
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

#include "Debug/Assert.h"
#include <cstdint>
#include <string_view>
namespace psych
{
enum class AssetType : uint16_t {
  None = 0,
  Scene,
  Prefab,
  Mesh,
  StaticMesh,
  MeshSource,
  Material,
  Texture,
  EnvMap,
  Audio,
  SoundConfig,
  SpatializationConfig,
  Font,
  Script,
  ScriptFile,
  MeshCollider,
  SoundGraphSound,
  Skeleton,
  Animation,
  AnimationGraph
};

namespace util
{

inline AssetType AssetTypeFromString(std::string_view assetType)
{
  if (assetType == "None")
    return AssetType::None;
  if (assetType == "Scene")
    return AssetType::Scene;
  if (assetType == "Prefab")
    return AssetType::Prefab;
  if (assetType == "Mesh")
    return AssetType::Mesh;
  if (assetType == "StaticMesh")
    return AssetType::StaticMesh;
  if (assetType == "MeshAsset")
    return AssetType::MeshSource; // DEPRECATED
  if (assetType == "MeshSource")
    return AssetType::MeshSource;
  if (assetType == "Material")
    return AssetType::Material;
  if (assetType == "Texture")
    return AssetType::Texture;
  if (assetType == "EnvMap")
    return AssetType::EnvMap;
  if (assetType == "Audio")
    return AssetType::Audio;
  if (assetType == "SoundConfig")
    return AssetType::SoundConfig;
  if (assetType == "Font")
    return AssetType::Font;
  if (assetType == "Script")
    return AssetType::Script;
  if (assetType == "ScriptFile")
    return AssetType::ScriptFile;
  if (assetType == "MeshCollider")
    return AssetType::MeshCollider;
  if (assetType == "SoundGraphSound")
    return AssetType::SoundGraphSound;
  if (assetType == "Skeleton")
    return AssetType::Skeleton;
  if (assetType == "Animation")
    return AssetType::Animation;
  // if (assetType == "AnimationController") return
  // AssetType::AnimationController; // OBSOLETE. You need to re-import animated
  // asset
  if (assetType == "AnimationGraph")
    return AssetType::AnimationGraph;

  return AssetType::None;
}

inline const char* AssetTypeToString(AssetType assetType)
{
  switch (assetType) {
  case AssetType::None:
    return "None";
  case AssetType::Scene:
    return "Scene";
  case AssetType::Prefab:
    return "Prefab";
  case AssetType::Mesh:
    return "Mesh";
  case AssetType::StaticMesh:
    return "StaticMesh";
  case AssetType::MeshSource:
    return "MeshSource";
  case AssetType::Material:
    return "Material";
  case AssetType::Texture:
    return "Texture";
  case AssetType::EnvMap:
    return "EnvMap";
  case AssetType::Audio:
    return "Audio";
  case AssetType::SoundConfig:
    return "SoundConfig";
  case AssetType::Font:
    return "Font";
  case AssetType::Script:
    return "Script";
  case AssetType::ScriptFile:
    return "ScriptFile";
  case AssetType::MeshCollider:
    return "MeshCollider";
  case AssetType::SoundGraphSound:
    return "SoundGraphSound";
  case AssetType::Skeleton:
    return "Skeleton";
  case AssetType::Animation:
    return "Animation";
  case AssetType::AnimationGraph:
    return "AnimationGraph";
  }

  CORE_ASSERT(false, "Unknown Asset Type");
  return "None";
}

} // namespace util
} // namespace psych
