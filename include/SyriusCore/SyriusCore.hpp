#pragma once

#include <SyriusCore/ExportCore.hpp>

#include <SyriusCore/Context/Context.hpp>

#include <SyriusCore/Window/SyriusWindow.hpp>

#include <SyriusCore/Image/Image.hpp>

#include <SyriusCore/Dependencies/imgui/imgui.h>
#include <SyriusCore/Dependencies/imgui/implot.h>


namespace Syrius {

    SR_CORE_API UP<SyriusWindow> createWindow(const WindowDesc& windowDesc);

}

