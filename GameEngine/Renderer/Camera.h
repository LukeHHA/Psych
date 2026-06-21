#pragma once

namespace ge
{
class Camera
{
public:
    virtual ~Camera();

    virtual const float* GetViewProjectionPtr() const = 0;
};
} // namespace ge
