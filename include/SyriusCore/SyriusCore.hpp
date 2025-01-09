#pragma once

#include "Utils/SyriusCoreInclude.hpp"
#include "ExportCore.hpp"

#include "Image/ImageBase.hpp"

#include "Window/SyriusWindow.hpp"

#include "Context/Context.hpp"

#include "Dependencies/imgui/imgui.h"

namespace Syrius {

    SR_CORE_API UP<SyriusWindow> createWindow(const WindowDesc& windowDesc);

}

