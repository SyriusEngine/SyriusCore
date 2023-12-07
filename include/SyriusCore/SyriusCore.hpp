#pragma once

#include "Utils/SyriusCoreInclude.hpp"
#include "Utils/Message.hpp"
#include "Utils/PlatformAPI.hpp"
#include "Utils/PlatformDetection.hpp"

#include "Utils/Image.hpp"
#include "Utils/Resource.hpp"

#include "Window/Window.hpp"

#include "Context/Context.hpp"
#include "Context/ShaderModule.hpp"
#include "Context/Shader.hpp"
#include "Context/VertexLayout.hpp"
#include "Context/VertexBuffer.hpp"
#include "Context/IndexBuffer.hpp"
#include "Context/VertexArray.hpp"
#include "Context/Texture2D.hpp"
#include "Context/ConstantBuffer.hpp"
#include "Context/Framebuffer.hpp"

#include "Dependencies/imgui/imgui.h"

namespace Syrius {

    SR_CORE_API Resource<Image> createImage(const std::string& fileName, bool flipOnLoad = true);

    SR_CORE_API Resource<Image> createImage(const ubyte* pixelData, int32 width, int32 height, SR_TEXTURE_FORMAT format);

    SR_CORE_API Resource<Window> createWindow(const WindowDesc& windowDesc);

    SR_CORE_API void setDebugMessageCallback(handleDebugMessageFunc callback);

    SR_CORE_API Size getAllocatedMemory();

    SR_CORE_API Size getFreedMemory();

    SR_CORE_API Size getMemoryUsage();

}

