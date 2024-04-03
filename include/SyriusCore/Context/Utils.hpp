#pragma once

#include "../Utils/SyriusCoreInclude.hpp"
#include "../Image/ImageFactory.hpp"
#include "../Utils/Resource.hpp"

// shader types
typedef enum SR_SHADER_TYPE {
    SR_SHADER_NONE                   = 0x00,
    SR_SHADER_VERTEX                 = 0x01,
    SR_SHADER_FRAGMENT               = 0x02,
    SR_SHADER_PIXEL                  = 0x02,
    SR_SHADER_GEOMETRY               = 0x03,
    SR_SHADER_TESSELATION_EVALUATION = 0x04,
    SR_SHADER_TESSELATION_CONTROL    = 0x05,
    SR_SHADER_COMPUTE                = 0x06
} SR_SHADER_TYPE;

typedef enum SR_SHADER_LANGUAGE_TYPE {
    SR_SHADER_LANGUAGE_NONE           = 0x00,
    SR_SHADER_LANGUAGE_GLSL           = 0x01,
    SR_SHADER_LANGUAGE_HLSL           = 0x02,
    SR_SHADER_LANGUAGE_SPRIV          = 0x03
} SR_SHADER_LANGUAGE_TYPE;

// buffer access types
typedef enum SR_BUFFER_USAGE {
    SR_BUFFER_USAGE_DEFAULT       = 0x01,
    SR_BUFFER_USAGE_DYNAMIC       = 0x02,
    SR_BUFFER_USAGE_STATIC        = 0x03

} SR_BUFFER_USAGE;

// drawing modes
typedef enum SR_DRAW_MODE {
    SR_DRAW_POINTS          = 0x01,
    SR_DRAW_LINES           = 0x02,
    SR_DRAW_LINES_STRIP     = 0x03,
    SR_DRAW_TRIANGLES       = 0x04,
    SR_DRAW_TRIANGLE_STRIP  = 0x05
} SR_DRAW_MODE;

// depth functions
typedef enum SR_COMPARISON_FUNC {
    SR_COMPARISON_FUNC_ALWAYS        = 0x01,
    SR_COMPARISON_FUNC_NEVER         = 0x02,
    SR_COMPARISON_FUNC_LESS          = 0x03,
    SR_COMPARISON_FUNC_EQUAL         = 0x04,
    SR_COMPARISON_FUNC_LESS_EQUAL    = 0x05,
    SR_COMPARISON_FUNC_GREATER       = 0x06,
    SR_COMPARISON_FUNC_GREATER_EQUAL = 0x07,
    SR_COMPARISON_FUNC_NOT_EQUAL     = 0x08
} SR_COMPARISON_FUNC;

typedef enum SR_DEPTH_MASK {
    SR_DEPTH_MASK_DISABLED  = 0x00,
    SR_DEPTH_MASK_ENABLED   = 0x01
} SR_DEPTH_MASK;

// stencil operations
typedef enum SR_STENCIL_FUNC{
    SR_STENCIL_FUNC_KEEP             = 0x01,
    SR_STENCIL_FUNC_ZERO             = 0x02,
    SR_STENCIL_FUNC_REPLACE          = 0x03,
    SR_STENCIL_FUNC_INCR             = 0x04,
    SR_STENCIL_FUNC_INCR_WRAP        = 0x05,
    SR_STENCIL_FUNC_DECR             = 0x06,
    SR_STENCIL_FUNC_DECR_WRAP        = 0x07,
    SR_STENCIL_FUNC_INVERT           = 0x08
} SR_STENCIL_FUNC;

typedef enum SR_TEXTURE_FILTER{
    SR_TEXTURE_FILTER_NONE      = 0x00,
    SR_TEXTURE_FILTER_LINEAR    = 0x01,
    SR_TEXTURE_FILTER_POINT     = 0x02
} SR_TEXTURE_FILTER;

typedef enum SR_TEXTURE_WRAP{
    SR_TEXTURE_WRAP_NONE            = 0x00,
    SR_TEXTURE_WRAP_REPEAT          = 0x01,
    SR_TEXTURE_WRAP_MIRROR_REPEAT   = 0x02,
    SR_TEXTURE_WRAP_CLAMP_EDGE      = 0x03,
    SR_TEXTURE_WRAP_CLAMP_BORDER    = 0x04
} SR_TEXTURE_ADDRESS_WRAP;

typedef enum SR_CUBEMAP_FACE{
    SR_CUBEMAP_FACE_RIGHT   = 0x00,
    SR_CUBEMAP_FACE_LEFT    = 0x01,
    SR_CUBEMAP_FACE_TOP     = 0x02,
    SR_CUBEMAP_FACE_BOTTOM  = 0x03,
    SR_CUBEMAP_FACE_FRONT    = 0x04,
    SR_CUBEMAP_FACE_BACK   = 0x05
} SR_CUBEMAP_FACE;

namespace Syrius{

    std::string SR_CORE_API getShaderTypeString(SR_SHADER_TYPE type);

}




