#pragma once

#include "cereal/cereal.hpp"

#include <string>

namespace psych
{
struct ProjectConfig {
  std::string Name           = "Default Project";
  std::string AssetDirectory = "data";

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(CEREAL_NVP(Name), CEREAL_NVP(AssetDirectory));
  }
};
} // namespace psych
