#include "OSAPI.h"
#include "Core/Core.h"
#include "OS/Platform/MacOS/MacOSAPI.h"

namespace psych
{
Unique<OSAPI> OSAPI::Create() { return CreateUnique<MacOSAPI>(); }
} // namespace psych
