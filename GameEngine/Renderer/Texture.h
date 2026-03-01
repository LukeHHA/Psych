#pragma once

#include <cstdint>

namespace ge
{
class Texture
{
public:
    virtual ~Texture();

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual uint32_t GetRendererID() const = 0;
};
} // namespace ge
