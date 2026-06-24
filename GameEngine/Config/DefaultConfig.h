#pragma once

#include "Renderer/RendererAPI.h"
#include "cereal/cereal.hpp"
#include <string>

namespace ge
{
struct EditorUISpec {
  bool EnableDocking            = true;
  bool EnableMultiViewports     = true;
  bool EnableKeyboardNavigation = true;
  bool EnableGamepadNavigation  = false;
  std::string FontPath;
  std::string ImGuiINIPath      = "config://imgui.ini";
  float FontSize                = 18.0f;

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(CEREAL_NVP(EnableDocking),
            CEREAL_NVP(EnableMultiViewports),
            CEREAL_NVP(EnableKeyboardNavigation),
            CEREAL_NVP(EnableGamepadNavigation),
            CEREAL_NVP(FontPath),
            CEREAL_NVP(ImGuiINIPath),
            CEREAL_NVP(FontSize));
  }
};

struct GameEngineSpecification {
  std::string Name             = "Application";
  RendererAPIType RenderingAPI = RendererAPIType::OPENGL;
  std::string AssetBasePath    = "assets://";
  bool EnableEditorUI          = true;
  EditorUISpec EditorUI;

  template <class Archive>
  void serialize(Archive& archive)
  {
    int renderingApi = static_cast<int>(RenderingAPI);
    archive(CEREAL_NVP(Name), cereal::make_nvp("RenderingAPI", renderingApi), CEREAL_NVP(EnableEditorUI), CEREAL_NVP(AssetBasePath), CEREAL_NVP(EditorUI));
    RenderingAPI = static_cast<RendererAPIType>(renderingApi);
  }
};
} // namespace ge
