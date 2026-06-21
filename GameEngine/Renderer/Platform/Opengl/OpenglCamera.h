#pragma once

#include "../../Camera.h"

namespace ge
{
class OpenglCamera : public Camera
{
public:
    OpenglCamera();
    ~OpenglCamera() override;

    const float* GetViewProjectionPtr() const override;
};
} // namespace ge
