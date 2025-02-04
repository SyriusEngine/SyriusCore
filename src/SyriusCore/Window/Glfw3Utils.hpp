#pragma once

#include "../../../include/SyriusCore/Window/Event/KeyboardEvent.hpp"
#include "../../../include/SyriusCore/Window/Event/MouseEvent.hpp"

#include "../Utils/PlatformInclude.hpp"
#include "../Utils/DebugMacros.hpp"

#if !defined(SR_PLATFORM_WIN64)

namespace Syrius {

    SR_KEYBOARD_KEY translateGlfwKey(int key);

    SR_MOUSE_BUTTON translateGlfwMouseButton(int button);


}

#endif