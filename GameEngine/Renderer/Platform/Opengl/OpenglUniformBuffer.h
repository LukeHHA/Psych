#pragma once

#include <cstdint>

#include "../../UniformBuffer.h"

namespace ge
{
class OpenglUniformBuffer : public UniformBuffer
{
public:
    OpenglUniformBuffer();
    ~OpenglUniformBuffer() override;

    void Bind(uint32_t binding) const override;

private:
    uint32_t m_RendererID = 0;
};
} // namespace ge
