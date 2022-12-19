#pragma once

#include "../../../include/SyriusCore/Core/PlatformDetection.hpp"
#include "../Core/DebugMacros.hpp"

#include "../../../include/SyriusCore/Window/Event/KeyboardEvent.hpp"

#if defined(SR_CORE_PLATFORM_LINUX)

namespace Syrius{

    SR_KEYBOARD_KEY convertVirtualKey(KeySym sym);

}

#endif

