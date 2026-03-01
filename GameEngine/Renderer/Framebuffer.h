#pragma once

#include <cstdint>

namespace ge
{
class Framebuffer
{
public:
    virtual ~Framebuffer();

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const = 0;
};
} // namespace ge
