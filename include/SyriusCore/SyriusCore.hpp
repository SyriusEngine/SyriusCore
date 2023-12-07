#pragma once

#include "Utils/SyriusCoreInclude.hpp"
#include "Utils/Message.hpp"
#include "Utils/PlatformAPI.hpp"
#include "Utils/PlatformDetection.hpp"

#include "Image/Image.hpp"
#include "Utils/Resource.hpp"

#include "Window/SyriusWindow.hpp"

#include "Context/Context.hpp"
#include "Context/ShaderModule.hpp"
#include "Context/Shader.hpp"
#include "Context/VertexLayout.hpp"
#include "Context/VertexBuffer.hpp"
#include "Context/IndexBuffer.hpp"
#include "Context/VertexArray.hpp"
#include "Context/Texture2D.hpp"
#include "Context/ConstantBuffer.hpp"
#include "Context/FrameBuffer.hpp"

#include "Dependencies/imgui/imgui.h"

namespace Syrius {

    SR_CORE_API Resource<SyriusWindow> createWindow(const WindowDesc& windowDesc);

    SR_CORE_API void setDebugMessageCallback(handleDebugMessageFunc callback);

    SR_CORE_API Size getAllocatedMemory();

    SR_CORE_API Size getFreedMemory();

    SR_CORE_API Size getMemoryUsage();

}

