#pragma once

#include "../Core/SyriusCoreInclude.hpp"
#include "../Utils/Image.hpp"

namespace Syrius{

    // basic types
    typedef enum SR_DATA_TYPE: uint8 {
        SR_TYPE_NONE    = 0x00,
        SR_VOID         = 0x10,
        SR_UINT8        = 0x20,
        SR_INT8         = 0x30,
        SR_UINT16       = 0x40,
        SR_INT16        = 0x50,
        SR_UINT32       = 0x60,
        SR_INT32        = 0x70,
        SR_UINT64       = 0x80,
        SR_INT64        = 0x90,
        SR_FLOAT16      = 0xA0,
        SR_FLOAT32      = 0xB0,
        SR_FLOAT64      = 0xC0,
    } SR_DATA_TYPE;

    // scalar types
    // upper 4 bits are the data type
    // lower 4 bits are the component count (NOTE: 0 represents 1 component, F represents 16 components)
    typedef enum SR_SCALAR_TYPE: uint8 {
        SR_UINT8_1      = 0x20,
        SR_UINT8_2      = 0x21,
        SR_UINT8_3      = 0x22,
        SR_UINT8_4      = 0x23,
        SR_UINT8_3X3    = 0x2B,
        SR_UINT8_4X4    = 0x2F,
        SR_INT8_1       = 0x30,
        SR_INT8_2       = 0x31,
        SR_INT8_3       = 0x32,
        SR_INT8_4       = 0x33,
        SR_INT8_3X3     = 0x3B,
        SR_INT8_4X4     = 0x3F,
        SR_UINT16_1     = 0x40,
        SR_UINT16_2     = 0x41,
        SR_UINT16_3     = 0x42,
        SR_UINT16_4     = 0x43,
        SR_UINT16_3X3   = 0x4B,
        SR_UINT16_4X4   = 0x4F,
        SR_INT16_1      = 0x50,
        SR_INT16_2      = 0x51,
        SR_INT16_3      = 0x52,
        SR_INT16_4      = 0x53,
        SR_INT16_3X3    = 0x5B,
        SR_INT16_4X4    = 0x5F,
        SR_UINT32_1     = 0x60,
        SR_UINT32_2     = 0x61,
        SR_UINT32_3     = 0x62,
        SR_UINT32_4     = 0x63,
        SR_UINT32_3X3   = 0x6B,
        SR_UINT32_4X4   = 0x6F,
        SR_INT32_1      = 0x70,
        SR_INT32_2      = 0x71,
        SR_INT32_3      = 0x72,
        SR_INT32_4      = 0x73,
        SR_INT32_3X3    = 0x7B,
        SR_INT32_4X4    = 0x7F,
        SR_UINT64_1     = 0x80,
        SR_UINT64_2     = 0x81,
        SR_UINT64_3     = 0x82,
        SR_UINT64_4     = 0x83,
        SR_UINT64_3X3   = 0x8B,
        SR_UINT64_4X4   = 0x8F,
        SR_INT64_1      = 0x90,
        SR_INT64_2      = 0x91,
        SR_INT64_3      = 0x92,
        SR_INT64_4      = 0x93,
        SR_INT64_3X3    = 0x9B,
        SR_INT64_4X4    = 0x9F,
        SR_FLOAT16_1    = 0xA0,
        SR_FLOAT16_2    = 0xA1,
        SR_FLOAT16_3    = 0xA2,
        SR_FLOAT16_4    = 0xA3,
        SR_FLOAT16_3X3  = 0xAB,
        SR_FLOAT16_4X4  = 0xAF,
        SR_FLOAT32_1    = 0xB0,
        SR_FLOAT32_2    = 0xB1,
        SR_FLOAT32_3    = 0xB2,
        SR_FLOAT32_4    = 0xB3,
        SR_FLOAT32_3X3  = 0xBB,
        SR_FLOAT32_4X4  = 0xBF,
        SR_FLOAT64_1    = 0xC0,
        SR_FLOAT64_2    = 0xC1,
        SR_FLOAT64_3    = 0xC2,
        SR_FLOAT64_4    = 0xC3,
        SR_FLOAT64_3X3  = 0xCB,
        SR_FLOAT64_4X4  = 0xCF,

    } SR_SCALAR_TYPE;

    // shader types
    typedef enum SR_SHADER_TYPE {
        SR_SHADER_VERTEX         = 0x01,
        SR_SHADER_FRAGMENT       = 0x02,
        SR_SHADER_PIXEL          = 0x02,
        SR_SHADER_GEOMETRY       = 0x03,
        SR_SHADER_TESSELATION    = 0x04,
        SR_SHADER_COMPUTE        = 0x05
    } SR_SHADER_TYPE;

    typedef enum SR_SHADER_CODE_TYPE {
        SR_SHADER_CODE_NONE           = 0x00,
        SR_SHADER_CODE_GLSL           = 0x01,
        SR_SHADER_CODE_HLSL           = 0x02,
        SR_SHADER_CODE_SPIRV          = 0x03
    } SR_SHADER_CODE_TYPE;

    // buffer access types
    typedef enum SR_BUFFER_TYPE {
        SR_BUFFER_DEFAULT       = 0x01,
        SR_BUFFER_DYNAMIC       = 0x02,
        SR_BUFFER_STATIC        = 0x03

    } SR_BUFFER_TYPE;

    // drawing modes
    typedef enum SR_DRAW_TYPE {
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

    // base texture formats, the upper 4 bits represent the number of channels
    typedef enum SR_TEXTURE_FORMAT: uint8 {
        SR_TEXTURE_FORMAT_R     = 0x10,
        SR_TEXTURE_FORMAT_RG    = 0x20,
        SR_TEXTURE_FORMAT_RGB   = 0x30,
        SR_TEXTURE_FORMAT_RGBA  = 0x40,
        SR_TEXTURE_FORMAT_BGR   = 0x31,
        SR_TEXTURE_FORMAT_BGRA  = 0x41
    } SR_TEXTURE_FORMAT;

    // Texture formats with data types, upper 4 bits represent the number of channels
    typedef enum SR_TEXTURE_DATA_FORMAT: uint8 {
        SR_TEXTURE_DATA_FORMAT_R8           = 0x11,
        SR_TEXTURE_DATA_FORMAT_R_I8         = 0x12,
        SR_TEXTURE_DATA_FORMAT_R_I16        = 0x13,
        SR_TEXTURE_DATA_FORMAT_R_I32        = 0x14,
        SR_TEXTURE_DATA_FORMAT_R_UI8        = 0x15,
        SR_TEXTURE_DATA_FORMAT_R_UI16       = 0x16,
        SR_TEXTURE_DATA_FORMAT_R_UI32       = 0x17,
        SR_TEXTURE_DATA_FORMAT_R_F16        = 0x18,
        SR_TEXTURE_DATA_FORMAT_R_F32        = 0x19,

        SR_TEXTURE_DATA_FORMAT_RG_8         = 0x21,
        SR_TEXTURE_DATA_FORMAT_RG_I8        = 0x22,
        SR_TEXTURE_DATA_FORMAT_RG_I16       = 0x23,
        SR_TEXTURE_DATA_FORMAT_RG_I32       = 0x24,
        SR_TEXTURE_DATA_FORMAT_RG_UI8       = 0x25,
        SR_TEXTURE_DATA_FORMAT_RG_UI16      = 0x26,
        SR_TEXTURE_DATA_FORMAT_RG_UI32      = 0x27,
        SR_TEXTURE_DATA_FORMAT_RG_F16       = 0x28,
        SR_TEXTURE_DATA_FORMAT_RG_F32       = 0x29,

        SR_TEXTURE_DATA_FORMAT_RGB_8        = 0x31,
        SR_TEXTURE_DATA_FORMAT_RGB_I8       = 0x32,
        SR_TEXTURE_DATA_FORMAT_RGB_I16      = 0x33,
        SR_TEXTURE_DATA_FORMAT_RGB_I32      = 0x34,
        SR_TEXTURE_DATA_FORMAT_RGB_UI8      = 0x35,
        SR_TEXTURE_DATA_FORMAT_RGB_UI16     = 0x36,
        SR_TEXTURE_DATA_FORMAT_RGB_UI32     = 0x37,
        SR_TEXTURE_DATA_FORMAT_RGB_F16      = 0x38,
        SR_TEXTURE_DATA_FORMAT_RGB_F32      = 0x39,

        SR_TEXTURE_DATA_FORMAT_RGBA_8       = 0x41,
        SR_TEXTURE_DATA_FORMAT_RGBA_I8      = 0x42,
        SR_TEXTURE_DATA_FORMAT_RGBA_I16     = 0x43,
        SR_TEXTURE_DATA_FORMAT_RGBA_I32     = 0x44,
        SR_TEXTURE_DATA_FORMAT_RGBA_UI8     = 0x45,
        SR_TEXTURE_DATA_FORMAT_RGBA_UI16    = 0x46,
        SR_TEXTURE_DATA_FORMAT_RGBA_UI32    = 0x47,
        SR_TEXTURE_DATA_FORMAT_RGBA_F16     = 0x48,
        SR_TEXTURE_DATA_FORMAT_RGBA_F32     = 0x49,

    } SR_TEXTURE_DATA_FORMAT;

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

    /**
     * @brief This function will return the size of a type in bytes
     * @example SR_UINT8 -> 1 byte
     * @example SR_UINT16 -> 2 bytes
     * @example SR_UINT32 -> 4 bytes
     * @example SR_UINT64 -> 8 bytes
     * @param type The type to get the size of
     * @return the number of bytes needed to store data of that type
     */
    uint8 SR_API getTypeSize(SR_DATA_TYPE type);
    uint8 SR_API getTypeSize(SR_SCALAR_TYPE type);

    /**
     * @brief Returns the number of components in a scalar type
     * @example SR_FLOAT32_1 -> 1
     * @example SR_FLOAT64_1 -> 1
     * @example SR_UINT8_4 -> 4
     * @param type
     * @return the number of components in the scalar type
     */
    uint8 SR_API getScalarComponentCount(SR_SCALAR_TYPE type);

    SR_DATA_TYPE SR_API getScalarDataType(SR_SCALAR_TYPE type);

    SR_TEXTURE_FORMAT SR_API getTextureFormat(SR_TEXTURE_DATA_FORMAT format);

    uint8 SR_API getTextureChannelCount(SR_TEXTURE_FORMAT format);

}




