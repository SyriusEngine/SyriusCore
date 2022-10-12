#pragma once

#include "Core/SyriusCoreInclude.hpp"
#include "Core/Message.hpp"
#include "Core/PlatformAPI.hpp"
#include "Core/PlatformDetection.hpp"

#include "Utils/Image.hpp"

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

    SR_API void syriusInit();

    SR_API void syriusTerminate();

    SR_API Time getStartupTime();

    SR_API Time getElapsedTimeSinceStart();

    SR_API Image* createImage(const std::string& fileName, bool flipOnLoad = true);

    SR_API Image* createImage(const ubyte* pixelData, int32 width, int32 height, int32 channelCount);

    SR_API SyriusWindow* createWindow(const WindowDesc& windowDesc);

    SR_API void setDebugMessageCallback(handleDebugMessageFunc callback);

    SR_API Time getTime();

    SR_API Time getTimeMilli();

    SR_API Size getAllocatedMemory();

    SR_API Size getFreedMemory();

    SR_API Size getMemoryUsage();

    template<typename T>
    SR_API T getRandom(T min, T max);

}

