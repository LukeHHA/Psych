#pragma once

#include <cstdint>

#include "../../Framebuffer.h"

namespace ge
{
class OpenglFramebuffer : public Framebuffer
{
public:
    OpenglFramebuffer();
    ~OpenglFramebuffer() override;

    void Bind() const override;
    void Unbind() const override;
    uint32_t GetColorAttachmentID(uint32_t index = 0) const override;

private:
    uint32_t m_RendererID = 0;
};
} // namespace ge
