
/**************************************************************************/
/*  DefaultConfig.h                                                       */
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

#include "Renderer/RendererAPI.h"
#include "cereal/cereal.hpp"
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <string>

namespace psych
{
struct EditorUISpec {
  bool EnableDocking            = true;
  bool EnableMultiViewports     = true;
  bool EnableKeyboardNavigation = true;
  bool EnableGamepadNavigation  = false;
  std::string FontPath          = "assets://data/fonts/JetBrainsMonoNerdFont-Regular.ttf";
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

struct ProjectSpec {
  std::string PreviousProjectsDir = "://";
  std::vector<std::string> PreviousProjects{"/Documents", "/Desktop"};

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(CEREAL_NVP(PreviousProjectsDir), CEREAL_NVP(PreviousProjects));
  }
};

struct PsychEngineSpecification {
  std::string Name             = "Application";
  RendererAPIType RenderingAPI = RendererAPIType::OPENGL;
  std::string AssetBasePath    = "assets://";
  bool EnableEditorUI          = true;
  EditorUISpec EditorUI;
  ProjectSpec Projects;

  template <class Archive>
  void serialize(Archive& archive)
  {
    int renderingApi = static_cast<int>(RenderingAPI);
    archive(CEREAL_NVP(Name),
            cereal::make_nvp("RenderingAPI", renderingApi),
            CEREAL_NVP(EnableEditorUI),
            CEREAL_NVP(AssetBasePath),
            CEREAL_NVP(EditorUI),
            CEREAL_NVP(Projects));
    RenderingAPI = static_cast<RendererAPIType>(renderingApi);
  }
};
} // namespace psych
