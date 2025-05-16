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
message(STATUS "SyriusUtils found in ${SYRIUS_UTILS_DIR}")

if (WIN32)
    if (MINGW)
        set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
        set(SYRIUS_CORE_LIBS -limm32 -ldwmapi -luser32 -ldxgi -lopenGL32 -ld3d11 -ld3dcompiler_47)

    elseif(MSVC)
        set(SYRIUS_CORE_LIBS imm32 dwmapi user32 dxgi dxguid openGL32 d3d11 d3dcompiler)
    endif()

elseif(UNIX)
    # On UNIX, because X11 and Wayland are way more effort to implement, we use GLFW for this.
    fetch_glfw()

    set(SYRIUS_CORE_LIBS -lGL glfw
    )
endif()

if (WIN32)
    set(SYRIUS_CORE_PLATFORM_SRC
        src/SyriusCore/Dependencies/imgui/imgui_impl_win32.cpp
        src/SyriusCore/Dependencies/imgui/imgui_impl_opengl3.cpp
        src/SyriusCore/Dependencies/imgui/imgui_impl_dx11.cpp

        src/SyriusCore/Dependencies/glad/src/wgl.c

        src/SyriusCore/Window/Win32/SyriusWindowWin32Impl.cpp
        src/SyriusCore/Window/Win32/Win32Utils.cpp

        src/SyriusCore/Context/OpenGL/Internal/WglContext.cpp

        src/SyriusCore/Context/D3D11/Internal/D3D11Buffer.cpp
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
        src/SyriusCore/Context/D3D11/D3D11ShaderStorageBuffer.cpp
    )

elseif(UNIX)
    set(SYRIUS_CORE_PLATFORM_SRC
        src/SyriusCore/Dependencies/imgui/imgui_impl_glfw.cpp
        src/SyriusCore/Dependencies/imgui/imgui_impl_opengl3.cpp

        src/SyriusCore/Window/SyriusWindowGlfw3Impl.cpp
        src/SyriusCore/Window/Glfw3Utils.cpp

        src/SyriusCore/Context/OpenGL/Internal/GlxContext.cpp
        src/SyriusCore/Context/OpenGL/Internal/Glfw3glContext.cpp
        src/SyriusCore/Context/OpenGL/Internal/Glfw3glContext.hpp
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
    src/SyriusCore/Context/ShaderStorageBuffer.cpp

    src/SyriusCore/Context/OpenGL/Internal/GlBuffer.cpp
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
    src/SyriusCore/Context/OpenGL/GlShaderStorageBuffer.cpp
)

add_library(SyriusCore SHARED ${SYRIUS_CORE_SRC})
if (UNIX)
    add_dependencies(SyriusCore glfw)
endif()
target_include_directories(SyriusCore PUBLIC include)
target_link_libraries(SyriusCore PUBLIC SyriusUtils)
target_link_libraries(SyriusCore PRIVATE ${SYRIUS_CORE_LIBS})

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
    Tests/Image/ImageUtilsTest.cpp
    Tests/Context/ShaderTest.cpp)

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTS)
    # Add GTest
    fetch_gtest()
    enable_testing()

    message(STATUS "Building tests")

    add_executable(SyriusCoreTest ${SYRIUS_CORE_SRC} ${SR_CORE_TEST_SRC} TestMain.cpp)
    add_dependencies(SyriusCoreTest gtest)

    target_link_libraries(SyriusCoreTest PUBLIC SyriusUtils)
    target_link_libraries(SyriusCoreTest PRIVATE ${SYRIUS_CORE_LIBS} GTest::gtest)

    add_test(NAME CTestSyriusCoreOpenGL COMMAND ${PROOT} $<TARGET_FILE:SyriusCoreTest> OpenGL)
    set_tests_properties(CTestSyriusCoreOpenGL PROPERTIES
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    add_test(NAME CTestSyriusCoreD3D11 COMMAND ${PROOT} $<TARGET_FILE:SyriusCoreTest> D3D11)
    set_tests_properties(CTestSyriusCoreD3D11 PROPERTIES
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif ()

################### Syrius Core App ###################

set(SYRIUS_CORE_APP_SRC
    SyriusCoreApp/SyriusCoreDev.cpp
    SyriusCoreApp/ShaderLibrary.cpp
    SyriusCoreApp/Utils.cpp
    SyriusCoreApp/Camera.cpp
    SyriusCoreApp/Layer.cpp
    SyriusCoreApp/Projection.cpp SyriusCoreApp/Material.cpp SyriusCoreApp/Material.hpp)

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
    SyriusCoreApp/Layers/DeferredPBRLayer.cpp
)

set(SYRIUS_CORE_SANDBOX_SRC
    SyriusCoreSandBox/SandBox.cpp
    SyriusCoreSandBox/Utils.cpp
    SyriusCoreSandBox/Utils.hpp

    SyriusCoreSandBox/Core/IComponentFactory.hpp
    SyriusCoreSandBox/Core/IComponent.hpp
    SyriusCoreSandBox/Core/IComponent.cpp
    SyriusCoreSandBox/Components/Debug.cpp
    SyriusCoreSandBox/Components/Debug.hpp
    SyriusCoreSandBox/Components/VertexDraw.cpp
    SyriusCoreSandBox/Components/VertexDraw.hpp
)


if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_APP)
    fetch_glm()
    message(STATUS "Building example App")

    add_executable(SyriusCoreSandBox main.cpp ${SYRIUS_CORE_SANDBOX_SRC})
    add_dependencies(SyriusCoreSandBox SyriusCore)
    target_link_libraries(SyriusCoreSandBox SyriusCore glm::glm)

    add_executable(SyriusCoreApp AppMain.cpp ${SYRIUS_CORE_APP_LAYERS_SRC})
    add_dependencies(SyriusCoreApp SyriusCore)
    target_link_libraries(SyriusCoreApp SyriusCore glm::glm)
endif()



