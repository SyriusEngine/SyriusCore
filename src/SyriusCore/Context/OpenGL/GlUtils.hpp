#pragma once

#include "../../../../include/SyriusCore/Context/Utils.hpp"
#include "../../Core/PlatformInclude.hpp"
#include "../../Core/DebugMacros.hpp"

namespace Syrius{

    inline uint64 generateID(){
        static uint64 id = 1;
        return id++;
    }

    GLenum getGlShaderType(SR_SHADER_TYPE type);

    GLenum getGlDrawType(SR_DRAW_TYPE type);

    GLenum getGlDataType(SR_TYPE type);

    GLenum getGlBufferType(SR_BUFFER_TYPE type);

    GLenum getGlTextureType(SR_CHANNEL_FORMAT type);

    GLint getGlTextureInternalFormat(SR_TEXTURE_FORMAT format);

    GLenum getGlComparisonFunc(SR_COMPARISON_FUNC func);

    GLint getGlTextureFilter(SR_TEXTURE_FILTER);

    GLint getGlTextureWrap(SR_TEXTURE_WRAP);

    GLint getGlStencilFunc(SR_STENCIL_FUNC);

    GLenum getGlTextureDataType(SR_TEXTURE_FORMAT format);

}

