#pragma once

#include "Core/Base.h"
namespace ge
{
class UILib
{
public:
  UILib()                                       = default;
  virtual ~UILib()                              = default;

  virtual void ToolTip(const std::string& desc) = 0;
  static Shared<UILib> Create();

private:
};

} // namespace ge
