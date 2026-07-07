#pragma once

namespace psych
{
class Camera
{
public:
  virtual ~Camera();

  virtual const float* GetViewProjectionPtr() const = 0;
};
} // namespace psych
