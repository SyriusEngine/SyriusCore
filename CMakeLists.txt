cmake_minimum_required(VERSION 3.16)
project(SyriusCore)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(SR_CORE_DEBUG)
add_compile_definitions(SR_CORE_EXPORT_DLL)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

if (WIN32)
    set(GLAD_PLATFORM_SRC
        src/SyriusCore/Dependencies/glad/src/wgl.c)

    set(IMGUI_PLATFORM_SRC
        src/SyriusCore/Dependencies/imgui/imgui_impl_win32.cpp
        src/SyriusCore/Dependencies/imgui/imgui_impl_dx11.cpp)

    set(SYRIUSCORE_PLATFORM_WINDOW_SRC
        src/SyriusCore/Window/SyriusWindowWin32Impl.cpp
        src/SyriusCore/Window/Win32Utils.cpp)

    set(SYRIUSCORE_PLATFORM_GRAPHICS_API_SRC
        src/SyriusCore/Context/OpenGL/Internal/WglContext.cpp

        src/SyriusCore/Context/D3D11/D3D11Context.cpp
        src/SyriusCore/Context/D3D11/D3D11Viewport.cpp
        src/SyriusCore/Context/D3D11/D3D11Utils.cpp
        src/SyriusCore/Context/D3D11/D3D11ShaderModule.cpp
        src/SyriusCore/Context/D3D11/D3D11Shader.cpp
        src/SyriusCore/Context/D3D11/D3D11VertexBuffer.cpp
        src/SyriusCore/Context/D3D11/D3D11IndexBuffer.cpp
        src/SyriusCore/Context/D3D11/D3D11VertexArray.cpp
        src/SyriusCore/Context/D3D11/D3D11ConstantBuffer.cpp
        src/SyriusCore/Context/D3D11/D3D11Texture2D.cpp
        src/SyriusCore/Context/D3D11/D3D11Sampler.cpp
        src/SyriusCore/Context/D3D11/D3D11FrameBuffer.cpp
        src/SyriusCore/Context/D3D11/D3D11ColorAttachment.cpp
        src/SyriusCore/Context/D3D11/D3D11DepthStencilAttachment.cpp
        src/SyriusCore/Context/D3D11/D3D11DeviceLimits.cpp
        src/SyriusCore/Context/D3D11/D3D11Cubemap.cpp)

elseif(UNIX)
    set(GLAD_PLATFORM_SRC
        src/SyriusCore/Dependencies/glad/src/glx.c)

    set(IMGUI_PLATFORM_SRC
        )

    set(SYRIUSCORE_PLATFORM_WINDOW_SRC
        src/SyriusCore/Window/SyriusWindowX11Impl.cpp
        src/SyriusCore/Window/X11Utils.cpp)

    set(SYRIUSCORE_PLATFORM_GRAPHICS_API_SRC
        src/SyriusCore/Context/OpenGL/Internal/GlxContext.cpp
        )

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
        src/SyriusCore/Utils/DebugMessageHandler.cpp
        src/SyriusCore/Utils/Message.cpp
        src/SyriusCore/Utils/MemoryAllocationTracker.cpp
        src/SyriusCore/Utils/SyriusCoreInclude.cpp)

set(SYRIUSCORE_IMAGE_SRC
    src/SyriusCore/Image/Image.cpp
    src/SyriusCore/Image/ImageFactory.cpp
    src/SyriusCore/Image/ImageUI8.cpp
    src/SyriusCore/Image/ImageF32.cpp
    src/SyriusCore/Image/ImageImpl.cpp
    src/SyriusCore/Image/ImageUI16.cpp
    src/SyriusCore/Image/InternalUtils.cpp)

set(SYRIUSCORE_WINDOW_SRC
    ${SYRIUSCORE_PLATFORM_WINDOW_SRC}
    src/SyriusCore/Window/Event/Event.cpp
    src/SyriusCore/Window/Event/MouseEvent.cpp
    src/SyriusCore/Window/Event/WindowEvent.cpp
    src/SyriusCore/Window/Event/KeyboardEvent.cpp
    src/SyriusCore/Window/SyriusWindow.cpp)

set(SYRIUSCORE_GRAPHICS_API_SRC
    ${SYRIUSCORE_PLATFORM_GRAPHICS_API_SRC}
    ${SYRIUSCORE_IMAGE_SRC}

    src/SyriusCore/Context/Context.cpp
    src/SyriusCore/Context/Viewport.cpp
    src/SyriusCore/Context/Utils.cpp
    src/SyriusCore/Context/ShaderModule.cpp
    src/SyriusCore/Context/Shader.cpp
    src/SyriusCore/Context/VertexLayout.cpp
    src/SyriusCore/Context/VertexBuffer.cpp
    src/SyriusCore/Context/IndexBuffer.cpp
    src/SyriusCore/Context/VertexArray.cpp
    src/SyriusCore/Context/Texture2D.cpp
    src/SyriusCore/Context/ConstantBuffer.cpp
    src/SyriusCore/Context/Sampler.cpp
    src/SyriusCore/Context/FrameBufferLayout.cpp
    src/SyriusCore/Context/FrameBuffer.cpp
    src/SyriusCore/Context/ColorAttachment.cpp
    src/SyriusCore/Context/DepthStencilAttachment.cpp
    src/SyriusCore/Context/Cubemap.cpp

    src/SyriusCore/Context/OpenGL/GlContext.cpp
    src/SyriusCore/Context/OpenGL/GlViewport.cpp
    src/SyriusCore/Context/OpenGL/GlUtils.cpp
    src/SyriusCore/Context/OpenGL/GlShaderModule.cpp
    src/SyriusCore/Context/OpenGL/GlShader.cpp
    src/SyriusCore/Context/OpenGL/GlVertexBuffer.cpp
    src/SyriusCore/Context/OpenGL/GlIndexBuffer.cpp
    src/SyriusCore/Context/OpenGL/GlVertexArray.cpp
    src/SyriusCore/Context/OpenGL/GlTexture2D.cpp
    src/SyriusCore/Context/OpenGL/GlConstantBuffer.cpp
    src/SyriusCore/Context/OpenGL/GlSampler.cpp
    src/SyriusCore/Context/OpenGL/GlFrameBuffer.cpp
    src/SyriusCore/Context/OpenGL/GlColorAttachment.cpp
    src/SyriusCore/Context/OpenGL/GlDepthStencilAttachment.cpp
    src/SyriusCore/Context/OpenGL/GlCubemap.cpp
    src/SyriusCore/Context/OpenGL/GlDeviceLimits.cpp)

set(SYRIUSCORE_SRC
    ${SYRIUSCORE_CORE_SRC}
    ${SYRIUSCORE_WINDOW_SRC}
    ${SYRIUSCORE_GRAPHICS_API_SRC}
    src/SyriusCore/SyriusCore.cpp include/SyriusCore/Utils/Resource.hpp)

add_library(SyriusCore SHARED ${SYRIUSCORE_SRC})

if (WIN32)
    if (MINGW)
        set(SYRIUS_CORE_LIBS
                -limm32
                -ldwmapi
                -luser32
                -ldxgi
                -lopenGL32
                -ld3d11
                -ld3dcompiler_47)

    elseif(MSVC)

        set(SYRIUS_CORE_LIBS
                imm32
                dwmapi
                user32
                dxgi
                dxguid
                openGL32
                d3d11
                d3dcompiler)
    endif()

elseif(UNIX)

    set(SYRIUS_CORE_LIBS
            -lX11
            -ldl
            -lGL)
endif()

target_link_libraries(SyriusCore ${SYRIUS_CORE_LIBS})


################### Syrius Core App ###################

include_directories(/include/)
include_directories(/glm/)
link_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SYRIUS_CORE_APP_SRC
    SyriusCoreApp/SyriusCoreDev.cpp
    SyriusCoreApp/ShaderLibrary.cpp
    SyriusCoreApp/Utils.cpp
    SyriusCoreApp/Camera.cpp
    SyriusCoreApp/Layer.cpp
    SyriusCoreApp/Projection.cpp)

set(SYRIUS_CORE_APP_LAYERS_SRC
    ${SYRIUS_CORE_APP_SRC}
    SyriusCoreApp/Layers/ImGuiLayer.cpp
    SyriusCoreApp/Layers/VertexBufferLayer.cpp
    SyriusCoreApp/Layers/VertexBufferLayer.hpp
    SyriusCoreApp/Layers/IndexBufferLayer.cpp SyriusCoreApp/Layers/IndexBufferLayer.hpp)

add_executable(SyriusCoreApp AppMain.cpp ${SYRIUS_CORE_APP_LAYERS_SRC})
add_dependencies(SyriusCoreApp SyriusCore)

if (MSVC)
    target_link_libraries(SyriusCoreApp SyriusCore)
else()
    target_link_libraries(SyriusCoreApp -lSyriusCore)
endif()



################### Testing ###################
include_directories(./Dependencies/gtest/include/)
link_directories(./Dependencies/gtest/lib/)

set(SR_CORE_TEST_SRC
    Tests/Utils/ConversionsTest.cpp
    Tests/TestEnvironment.cpp

    Tests/Context/VertexBufferTest.cpp
    Tests/Context/IndexBufferTest.cpp
    Tests/Context/ConstantBufferTest.cpp
    Tests/Context/VertexLayoutTest.cpp
    Tests/Context/ShaderModuleTest.cpp

    Tests/Context/OpenGL/GLUtilsTest.cpp

    Tests/Context/D3D11/D3D11UtilsTest.cpp
    Tests/Context/Texture2DTest.cpp
    Tests/Context/Texture2DTest.hpp
)

add_executable(SyriusCoreTest ${SYRIUSCORE_SRC} ${SR_CORE_TEST_SRC} TestMain.cpp)

if (MSVC)
    target_link_libraries(SyriusCoreTest ${SYRIUS_CORE_LIBS} gtest)
else()
    target_link_libraries(SyriusCoreTest ${SYRIUS_CORE_LIBS} -lgtest)
endif()


