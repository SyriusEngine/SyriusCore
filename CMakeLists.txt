cmake_minimum_required(VERSION 3.16)
project(SyriusCore)

# Check if SyriusBuildUtils has been initialized
if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/SyriusBuildUtils/CMakeUtils.cmake")
    message("SyriusBuildUtils not found, cloning from git...")
    execute_process(COMMAND git submodule init
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE INIT_RESULT)
    if(INIT_RESULT)
        message(FATAL_ERROR "Failed to init SyriusBuildUtils")
    endif()
    execute_process(COMMAND git submodule update
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE UPDATE_RESULT)
    if(UPDATE_RESULT)
        message(FATAL_ERROR "Failed to update SyriusBuildUtils")
    endif()
else()
    message(STATUS "SyriusBuildUtils found!")
endif()
include(SyriusBuildUtils/CMakeUtils.cmake)

# Find the SyriusUtils repo (locally or from git)
set(SYRIUS_UTILS_DIR)
include_or_fetch(SyriusUtils SYRIUS_UTILS_DIR)
message("SyriusUtils found in ${SYRIUS_UTILS_DIR}")

# Then, if we are on Windows, copy and paste it into the current bin dir
# Nifty trick to get the name of the build folder of the other dir
get_filename_component(BUILD_FOLDER_NAME ${CMAKE_BINARY_DIR} NAME)

if (WIN32)
    if (MINGW)
        set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
        set(SYRIUS_CORE_LIBS
            -limm32
            -ldwmapi
            -luser32
            -ldxgi
            -lopenGL32
            -ld3d11
            -ld3dcompiler_47
            SyriusUtils
            )

        set(SYRIUS_UTILS_DLL "${SYRIUS_UTILS_DIR}/${BUILD_FOLDER_NAME}/libSyriusUtils.dll")

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
        set(SYRIUS_UTILS_DLL "${SYRIUS_UTILS_DIR}/${BUILD_FOLDER_NAME}/SyriusUtils.dll")
    endif()

elseif(UNIX)
    set(SYRIUS_CORE_LIBS
        -lX11
        -ldl
        -lGL)
endif()

add_custom_target(CopySyriusUtilsDLL ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SYRIUS_UTILS_DLL} ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${SYRIUS_UTILS_DLL}
    COMMENT "Copying ${SYRIUS_UTILS_DLL} to build tree")
add_dependencies(CopySyriusUtilsDLL SyriusUtils)

add_compile_definitions(SR_CORE_DEBUG)
add_compile_definitions(SR_CORE_EXPORT_DLL)

if (WIN32)
    set(SYRIUS_CORE_PLATFORM_SRC
        src/SyriusCore/Dependencies/glad/src/wgl.c

        src/SyriusCore/Dependencies/imgui/imgui_impl_win32.cpp
        src/SyriusCore/Dependencies/imgui/imgui_impl_dx11.cpp
        src/SyriusCore/Dependencies/imgui/imgui_impl_opengl3.cpp

        src/SyriusCore/Window/SyriusWindowWin32Impl.cpp
        src/SyriusCore/Window/Win32Utils.cpp

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
        src/SyriusCore/Context/D3D11/D3D11Cubemap.cpp
        )

elseif(UNIX)
    set(SYRIUS_CORE_PLATFORM_SRC
        src/SyriusCore/Dependencies/glad/src/glx.c

        src/SyriusCore/Dependencies/imgui/imgui_impl_opengl3.cpp

        src/SyriusCore/Window/SyriusWindowX11Impl.cpp
        src/SyriusCore/Window/X11Utils.cpp

        src/SyriusCore/Context/OpenGL/Internal/GlxContext.cpp
        )

endif()

set(SYRIUS_CORE_SRC
    src/SyriusCore/SyriusCore.cpp

    ${SYRIUS_CORE_PLATFORM_SRC}
    src/SyriusCore/Dependencies/glad/src/gl.c

    src/SyriusCore/Dependencies/imgui/imgui.cpp
    src/SyriusCore/Dependencies/imgui/imgui_demo.cpp
    src/SyriusCore/Dependencies/imgui/imgui_draw.cpp
    src/SyriusCore/Dependencies/imgui/imgui_tables.cpp
    src/SyriusCore/Dependencies/imgui/imgui_widgets.cpp

    src/SyriusCore/Dependencies/stb_image/stb_image.cpp
    src/SyriusCore/Dependencies/stb_image/stb_image_resize.cpp
    src/SyriusCore/Dependencies/stb_image/stb_image_write.cpp

    src/SyriusCore/Utils/CoreLogger.cpp
    src/SyriusCore/Utils/SyriusCoreInclude.cpp
    src/SyriusCore/Utils/CoreLoggerDefs.cpp

    src/SyriusCore/Image/ImageBase.cpp
    src/SyriusCore/Image/Image.cpp
    src/SyriusCore/Image/ImageUI8.cpp
    src/SyriusCore/Image/ImageF32.cpp
    src/SyriusCore/Image/ImageUI16.cpp
    src/SyriusCore/Image/InternalImageUtils.cpp
    src/SyriusCore/Image/ImageT.cpp

    src/SyriusCore/Window/Event/Event.cpp
    src/SyriusCore/Window/Event/MouseEvent.cpp
    src/SyriusCore/Window/Event/WindowEvent.cpp
    src/SyriusCore/Window/Event/KeyboardEvent.cpp
    src/SyriusCore/Window/SyriusWindow.cpp

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
    src/SyriusCore/Context/CubeMap.cpp
    src/SyriusCore/Context/CubeMapLayout.cpp

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
    src/SyriusCore/Context/OpenGL/GlCubeMap.cpp
    src/SyriusCore/Context/OpenGL/GlDeviceLimits.cpp
    )


add_library(SyriusCore SHARED ${SYRIUS_CORE_SRC})
add_dependencies(SyriusCore CopySyriusUtilsDLL)
target_include_directories(SyriusCore PUBLIC include)
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
    SyriusCoreApp/Layers/IndexBufferLayer.cpp
    SyriusCoreApp/Layers/VertexArrayLayer.cpp
    SyriusCoreApp/Layers/ConstantBufferLayer.cpp
    SyriusCoreApp/Layers/SamplerLayer.cpp
    SyriusCoreApp/Layers/TextureLayer.cpp
    SyriusCoreApp/Layers/DepthTestLayer.cpp
    SyriusCoreApp/Layers/TextureAtlasLayer.cpp
    SyriusCoreApp/Layers/FrameBufferLayer.cpp
    SyriusCoreApp/Layers/ShaderLayer.cpp
    SyriusCoreApp/Layers/CubeMapLayer.cpp
)

add_executable(SyriusCoreApp AppMain.cpp ${SYRIUS_CORE_APP_LAYERS_SRC})
add_dependencies(SyriusCoreApp SyriusCore)

if (MSVC)
    target_link_libraries(SyriusCoreApp SyriusCore)
else()
    target_link_libraries(SyriusCoreApp -lSyriusCore)
endif()

################### Testing ###################
set(SR_CORE_TEST_SRC
    Tests/Utils/ConversionsTest.cpp
    Tests/TestEnvironment.cpp

    Tests/Context/VertexBufferTest.cpp
    Tests/Context/IndexBufferTest.cpp
    Tests/Context/ConstantBufferTest.cpp
    Tests/Context/VertexLayoutTest.cpp
    Tests/Context/ShaderModuleTest.cpp
    Tests/Context/Texture2DTest.cpp
    Tests/Context/FramebufferLayoutTest.cpp
    Tests/Context/CubeMapLayoutTest.cpp
    Tests/Context/FrameBufferTest.cpp
    Tests/Context/CubeMapTest.cpp

    Tests/Context/OpenGL/GLUtilsTest.cpp

    Tests/Context/D3D11/D3D11UtilsTest.cpp

    Tests/Image/ImageUI8Test.cpp
    Tests/Image/ImageF32Test.cpp
    Tests/Image/ImageUtilsTest.cpp)

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTS)
    # Add GTest
    set(GTEST_DIR "${CMAKE_CURRENT_LIST_DIR}/Dependencies/gtest")
    if (NOT EXISTS ${GTEST_DIR})
        message("GTest not found, cloning from git...")
        execute_process(COMMAND git clone https://github.com/google/googletest ${GTEST_DIR}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE result)
        if(result)
            message(FATAL_ERROR "Failed to download GTest")
        endif()
    else()
        message(STATUS "GTest found!")
    endif ()
    add_subdirectory(${GTEST_DIR})

    message(STATUS "Building tests")

    add_executable(SyriusCoreTest ${SYRIUS_CORE_SRC} ${SR_CORE_TEST_SRC} TestMain.cpp)
    add_dependencies(SyriusCoreTest gtest)

    target_link_libraries(SyriusCoreTest  ${SYRIUS_CORE_LIBS} GTest::gtest)
endif ()


