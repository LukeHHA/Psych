#pragma once

#include "FileSystem/FileSystem.h"
#include "Renderer/RendererAPI.h"

namespace ge::config
{
struct EditorUISpec {
  bool EnableDocking            = true;
  bool EnableMultiViewports     = true;
  bool EnableKeyboardNavigation = true;
  bool EnableGamepadNavigation  = false;
  util::FilePath FontPath;
  float FontSize = 18.0f;
};

struct GameEngineSpecification {
  std::string Name             = "Application";
  RendererAPIType RenderingAPI = RendererAPIType::OPENGL;
  util::FilePath AssetBasePath;
  bool EnableEditorUI = false;
  EditorUISpec EditorUI;
};
} // namespace ge::config
