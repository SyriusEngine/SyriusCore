#pragma once

#include <SyriusUtils/SyriusUtils.hpp>
#include "../Utils/DebugMacros.hpp"

#include "../../../include/SyriusCore/Window/Event/KeyboardEvent.hpp"

#if defined(SR_CORE_PLATFORM_LINUX)

namespace Syrius{

    SR_KEYBOARD_KEY convertVirtualKey(KeySym sym);

}

#endif

