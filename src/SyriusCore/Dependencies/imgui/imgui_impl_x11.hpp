#pragma once

#include "../../../../include/SyriusCore/Dependencies/imgui/imgui.h"      // IMGUI_IMPL_API

IMGUI_IMPL_API bool ImGui_ImplX11_Init(void* display, void* window);
IMGUI_IMPL_API void ImGui_ImplX11_Shutdown();
IMGUI_IMPL_API void ImGui_ImplX11_NewFrame();