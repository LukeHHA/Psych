#pragma once

#include "Core/Base.h"
#include "UI/UILib.h"
#include <string>

namespace ge
{
class UI
{
public:
  UI();
  ~UI() = default;

  void ToolTip(const std::string& desc) { s_UILib->ToolTip(desc); }

private:
  Shared<UILib> s_UILib = nullptr;
};
} // namespace ge
