cmake_minimum_required(VERSION 3.21)
project(SyriusCore)

add_compile_definitions(SR_DEBUG_MODE)

set(CMAKE_CXX_STANDARD 17)

include_directories(Dependencies/Vulkan/1.3.211.0/Include/)
link_directories(Dependencies/Vulkan/1.3.211.0/Bin/)

if (WIN32)
    set(GLAD_PLATFORM_SRC
            src/SyriusCore/Dependencies/glad/src/wgl.c)

    set(IMGUI_PLATFORM_SRC
            src/SyriusCore/Dependencies/imgui/imgui_impl_win32.cpp
#            src/SyriusCore/Dependencies/imgui/imgui_impl_dx12.cpp
            src/SyriusCore/Dependencies/imgui/imgui_impl_dx11.cpp)

    set(SYRIUSCORE_CORE_PLATFORM_SRC
            src/SyriusCore/Core/PlatformAPIWin32Impl.cpp)


endif()

set(GLAD_SRC
        ${GLAD_PLATFORM_SRC}
        src/SyriusCore/Dependencies/glad/src/gl.c)

set(IMGUI_SRC
        ${IMGUI_PLATFORM_SRC}
        src/SyriusCore/Dependencies/imgui/imgui.cpp
        src/SyriusCore/Dependencies/imgui/imgui_demo.cpp
        src/SyriusCore/Dependencies/imgui/imgui_draw.cpp
        src/SyriusCore/Dependencies/imgui/imgui_impl_opengl3.cpp
        src/SyriusCore/Dependencies/imgui/imgui_tables.cpp
        src/SyriusCore/Dependencies/imgui/imgui_widgets.cpp)

set(STB_IMAGE_SRC
        src/SyriusCore/Dependencies/stb_image/stb_image.cpp
        src/SyriusCore/Dependencies/stb_image/stb_image_resize.cpp
        src/SyriusCore/Dependencies/stb_image/stb_image_write.cpp)

set(SYRIUSCORE_CORE_SRC
        ${GLAD_SRC}
        ${IMGUI_SRC}
        ${STB_IMAGE_SRC}
        ${SYRIUSCORE_CORE_PLATFORM_SRC}
        src/SyriusCore/Core/DebugMessageHandler.cpp
        src/SyriusCore/Core/PlatformAPI.cpp
        src/SyriusCore/Core/CoreCommand.cpp
        src/SyriusCore/Core/Message.cpp
        src/SyriusCore/Core/Vulkan/VulkanInstance.cpp
        src/SyriusCore/Core/Vulkan/VulkanPhysicalDevice.cpp
        src/SyriusCore/Core/Vulkan/VulkanInterface.cpp
        src/SyriusCore/Core/Vulkan/VulkanQueueFamily.cpp
        src/SyriusCore/Core/Vulkan/VulkanLogicalDevice.cpp)

set(SYRIUSCORE_UTILS_SRC
        src/SyriusCore/Utils/Image.cpp)

set(SYRIUSCORE_WINDOW_SRC
        src/SyriusCore/Window/Event/Event.cpp
        src/SyriusCore/Window/Event/MouseEvent.cpp
        src/SyriusCore/Window/Event/WindowEvent.cpp
        src/SyriusCore/Window/Event/KeyboardEvent.cpp
        src/SyriusCore/Window/SyriusWindow.cpp
        src/SyriusCore/Window/SyriusWindowWin32Impl.cpp
        src/SyriusCore/Window/Win32Utils.cpp
        src/SyriusCore/Window/Context/Context.cpp
        src/SyriusCore/Window/Context/GlContext.cpp
        src/SyriusCore/Window/Context/WglContext.cpp src/SyriusCore/Window/Context/VulkanContext.cpp src/SyriusCore/Window/Context/VulkanContext.hpp src/SyriusCore/Window/Context/VulkanContextWin32.cpp)

set(SYRIUSCORE_SRC
        ${SYRIUSCORE_CORE_SRC}
        ${SYRIUSCORE_UTILS_SRC}
        ${SYRIUSCORE_WINDOW_SRC}
        src/SyriusCore/SyriusCore.cpp)

add_library(syrius_core SHARED ${SYRIUSCORE_SRC})

if (WIN32)
    file(GLOB VULKAN_DLL "./Dependencies/Vulkan/1.3.211.0/Bin/*.dll")
    file(COPY ${VULKAN_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    if (MINGW)
        message("Platform: Win32, toolchain: MinGW")
        set(SYRIUS_CORE_LIBS
                -limm32
                -ldwmapi
                -luser32
                -ldxgi
                -lvulkan-1
                -lopenGL32
                -ld3d11
                -ld3dcompiler_47)

    elseif(MSVC)
        message("Platform: Win32, toolchain: MSVC")

        set(SYRIUS_CORE_LIBS
                imm32
                dwmapi
                user32
                dxgi
                vulkan-1
                openGL32
                d3d11
                d3dcompiler)
    endif()

elseif(UNIX)
    message("Platform: Unix, toolchain: unknown")
endif()

target_link_libraries(syrius_core ${SYRIUS_CORE_LIBS})

include_directories(/include/)
link_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(SyriusCoreDev main.cpp)
target_link_libraries(SyriusCoreDev -lsyrius_core)
