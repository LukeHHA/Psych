#pragma once

#include "../../Camera.h"

namespace psych
{
class OpenglCamera : public Camera
{
public:
  OpenglCamera();
  ~OpenglCamera() override;

  const float* GetViewProjectionPtr() const override;
};
} // namespace psych
