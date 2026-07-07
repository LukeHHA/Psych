#pragma once

#include <cstdint>

namespace psych
{
class UniformBuffer
{
public:
  virtual ~UniformBuffer();

  virtual void Bind(uint32_t binding) const = 0;
};
} // namespace psych
