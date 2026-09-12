#include "OSAPI.h"
#include "Core/Core.h"
#include "OS/Platform/Linux/LinuxOSAPI.h"
#include "OS/Platform/MacOS/MacOSAPI.h"

namespace psych
{
Unique<OSAPI> OSAPI::Create()
{
#if defined(__APPLE__)
  return CreateUnique<MacOSAPI>();
#elif defined(__linux__)
  return CreateUnique<LinuxOSAPI>();
#endif
}
} // namespace psych
