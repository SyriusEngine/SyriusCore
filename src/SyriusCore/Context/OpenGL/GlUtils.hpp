#pragma once

#include "../../../../include/SyriusCore/Context/Utils.hpp"
#include "../../Utils/PlatformInclude.hpp"
#include "../../Utils/DebugMacros.hpp"

namespace Syrius{

    inline uint64 generateID(){
        static uint64 id = 1;
        return id++;
    }

    GLenum getGlDrawType(SR_DRAW_MODE type);

    GLenum getGlDataType(SR_TYPE type);

    GLenum getGlShaderType(SR_SHADER_TYPE type);

    GLenum getGlBufferType(SR_BUFFER_USAGE type);

    GLenum getGlChannelFormat(SR_TEXTURE_FORMAT type);

    GLint getGlTextureFormat(SR_TEXTURE_FORMAT format);

    GLint getGlTextureFormatSized(SR_TEXTURE_FORMAT format);

    GLenum getGlTextureDataType(SR_TEXTURE_FORMAT format);

    GLenum getGlComparisonFunc(SR_COMPARISON_FUNC func);

    GLint getGlTextureFilter(SR_TEXTURE_FILTER);

    GLint getGlTextureWrap(SR_TEXTURE_WRAP);

    GLint getGlStencilFunc(SR_STENCIL_FUNC);

}

