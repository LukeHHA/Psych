#include "UILib.h"

namespace ge
{
Shared<UILib> UILib::Create() { return CreateShared<ImGuiUILib>(); }
} // namespace ge
