#pragma once

#include "../../../../include/SyriusCore/Context/Utils.hpp"
#include "../../Core/PlatformInclude.hpp"
#include "../../Core/DebugMacros.hpp"

namespace Syrius{

    GLenum getGlShaderType(SR_SHADER_TYPE type);

    GLenum getGlDrawType(SR_DRAW_TYPE type);

    GLenum getGlDataType(SR_DATA_TYPE type);

    GLenum getGlBufferType(SR_BUFFER_TYPE type);

}

