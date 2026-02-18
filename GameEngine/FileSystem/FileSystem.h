#pragma once

#include "Core/Base.h"

namespace ge::util
{
class Filesystem
{
public:
  Filesystem()  = delete;
  ~Filesystem() = delete;
  CORE_DELETE_COPY_OPS(Filesystem);

  static void Init();
  static void Shutdown();

private:
};
} // namespace ge::util
