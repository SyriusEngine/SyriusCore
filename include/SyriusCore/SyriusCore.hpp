#pragma once

#include "Core/SyriusCoreInclude.hpp"
#include "Core/Message.hpp"
#include "Core/PlatformAPI.hpp"
#include "Core/PlatformDetection.hpp"

#include "Utils/Image.hpp"
#include "Utils/Resource.hpp"

#include "Window/SyriusWindow.hpp"

#include "Context/Context.hpp"
#include "Context/ShaderModule.hpp"
#include "Context/Shader.hpp"
#include "Context/VertexDescription.hpp"
#include "Context/VertexBuffer.hpp"
#include "Context/IndexBuffer.hpp"
#include "Context/VertexArray.hpp"
#include "Context/Texture2D.hpp"
#include "Context/ConstantBuffer.hpp"
#include "Context/FrameBuffer.hpp"

#include "Dependencies/imgui/imgui.h"

namespace Syrius {

    SR_CORE_API void syriusCoreInit();

    SR_CORE_API void syriusCoreTerminate();

    SR_CORE_API Time getStartupTime();

    SR_CORE_API Time getElapsedTimeSinceStart();

    SR_CORE_API Resource<Image> createImage(const std::string& fileName, bool flipOnLoad = true);

    SR_CORE_API Resource<Image> createImage(const ubyte* pixelData, int32 width, int32 height, int32 channelCount);

    SR_CORE_API Resource<SyriusWindow> createWindow(const WindowDesc& windowDesc);

    SR_CORE_API void setDebugMessageCallback(handleDebugMessageFunc callback);

    SR_CORE_API Time getTime();

    SR_CORE_API Time getTimeMilli();

    SR_CORE_API Size getAllocatedMemory();

    SR_CORE_API Size getFreedMemory();

    SR_CORE_API Size getMemoryUsage();

    template<typename T>
    SR_CORE_API T getRandom(T min, T max);

}

