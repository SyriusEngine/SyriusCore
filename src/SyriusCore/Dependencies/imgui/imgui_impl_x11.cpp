// dear imgui: Platform Backend for UNIX systems that use X11 Windowing System
// This needs to be used along with a Renderer (e.g. OpenGL3, Vulkan..)

// Implemented features:
//  [] Platform: Clipboard support (for Win32 this is actually part of core dear imgui)
//  [] Platform: Keyboard support. Since 1.87 we are using the io.AddKeyEvent() function. Pass ImGuiKey values to all key functions e.g. ImGui::IsKeyPressed(ImGuiKey_Space). [Legacy VK_* values will also be supported unless IMGUI_DISABLE_OBSOLETE_KEYIO is set]
//  [] Platform: Gamepad support. Enabled with 'io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad'.
//  [] Platform: Mouse cursor shape and visibility. Disable with 'io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange'.
//  [] Platform: Multi-viewport support (multiple windows). Enable with 'io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable'.

#include "../../../../include/SyriusCore/Dependencies/imgui/imgui.h"
#include "imgui_impl_win32.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

struct ImGui_ImplX11_Data {
    Display*    Display;
    Window      Window;
    bool                        MouseTracked;
    int                         MouseButtonsDown;
    u_int64_t                  Time;
    u_int64_t                  TicksPerSecond;

    ImGuiMouseCursor            LastMouseCursor;
    bool                        HasGamepad;
    bool                        WantUpdateHasGamepad;
    bool                        WantUpdateMonitors;

    ImGui_ImplX11_Data() { memset((void*)this, 0, sizeof(*this)); }
};

static ImGui_ImplX11_Data* ImGui_ImplWin32_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplX11_Data*) ImGui::GetIO().BackendPlatformUserData : NULL;
}

bool ImGui_ImplX11_Init(void* display, void* window) {
    ImGuiIO& io = ImGui::GetIO();

    IM_ASSERT(io.BackendPlatformUserData == NULL && "Already initialized a platform backend!");

    // Setup backend capabilities flags
    ImGui_ImplX11_Data* bd = IM_NEW(ImGui_ImplX11_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "imgui_impl_x11";
//    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
//    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
//    io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;    // We can create multi-viewports on the Platform side (optional)
//    io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport; // We can call io.AddMouseViewportEvent() with correct data (optional)
//    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;              // We can use io.NavInputs[]

    bd->Display = (Display*)display;
    bd->Window = (Window)window;
    bd->WantUpdateHasGamepad = true;
    bd->WantUpdateMonitors = true;
    // TODO: Is this necessary?
//    bd->TicksPerSecond = perf_frequency;
//    bd->Time = perf_counter;
    bd->LastMouseCursor = ImGuiMouseCursor_COUNT;

    return true;
}


