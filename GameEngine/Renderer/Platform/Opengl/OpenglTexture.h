#pragma once

#include <cstdint>

#include "../../Texture.h"

namespace ge
{
class OpenglTexture : public Texture
{
public:
    OpenglTexture();
    ~OpenglTexture() override;

    void Bind(uint32_t slot = 0) const override;
    uint32_t GetRendererID() const override;

private:
    uint32_t m_RendererID = 0;
};
} // namespace ge
