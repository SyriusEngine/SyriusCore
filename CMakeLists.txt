cmake_minimum_required(VERSION 3.21)
project(SyriusCore)

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

set(SYRIUS_CORE_SRC
        ${GLAD_SRC}
        ${IMGUI_SRC}
        ${STB_IMAGE_SRC}
        src/SyriusCore/SyriusCore.cpp)

add_library(syrius_core SHARED ${SYRIUS_CORE_SRC})

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

add_executable(SyriusCore main.cpp)
target_link_libraries(SyriusCore -lsyrius_core)
