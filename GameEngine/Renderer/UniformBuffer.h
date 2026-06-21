#pragma once

#include <cstdint>

namespace ge
{
class UniformBuffer
{
public:
    virtual ~UniformBuffer();

    virtual void Bind(uint32_t binding) const = 0;
};
} // namespace ge
