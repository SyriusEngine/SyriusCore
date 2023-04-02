#pragma once

#include "PlatformDetection.hpp"

#include <vector>
#include <string>
#include <deque>

#define SR_DEFAULT_WIDTH 1280
#define SR_DEFAULT_HEIGHT 720

namespace Syrius {

    typedef enum SR_SUPPORTED_API {
        SR_API_NONE = 0x00,
        SR_API_OPENGL = 0x01,
        SR_API_VULKAN = 0x10,

#if defined(SR_CORE_PLATFORM_WIN64)
        SR_API_D3D11 = 0x08,
        SR_API_D3D12 = 0x09,
#elif defined(SR_CORE_PLATFORM_LINUX)
        SR_API_D3D11 = ~0x08,
        SR_API_D3D12 = ~0x09,

#endif

    } SR_SUPPORTED_API;

    inline std::string getAPIName(SR_SUPPORTED_API api) {
        switch (api) {
            case SR_API_NONE:
                return "None";
            case SR_API_OPENGL:
                return "OpenGL";
            case SR_API_VULKAN:
                return "Vulkan";
#if defined(SR_CORE_PLATFORM_WIN64)
            case SR_API_D3D11:
                return "DirectX 11";
            case SR_API_D3D12:
                return "DirectX 12";
#endif
            default:
                return "Unknown";
        }
    }

    typedef enum SR_PHYSICAL_DEVICE_TYPE {
        SR_PHYSICAL_DEVICE_NONE = 0x00,
        SR_PHYSICAL_DEVICE_INTEGRATED_GPU = 0x01,
        SR_PHYSICAL_DEVICE_DISCRETE_GPU = 0x02,
        SR_PHYSICAL_DEVICE_VIRTUAL_GPU = 0x03,
        SR_PHYSICAL_DEVICE_CPU = 0x04,
    } SR_PHYSICAL_DEVICE_TYPE;

    typedef enum SR_LOAD_TYPE {
        SR_LOAD_NONE = 0x00,
        SR_LOAD_FROM_FILE = 0x01,
        SR_LOAD_FROM_MEMORY = 0x02,
    } SR_LOAD_TYPE;

    typedef uint8_t uint8;
    typedef int8_t int8;
    typedef uint16_t uint16;
    typedef int16_t int16;
    typedef uint32_t uint32;
    typedef int32_t int32;
    typedef uint64_t uint64;
    typedef int64_t int64;
    typedef unsigned char ubyte;
    typedef char byte;

    typedef size_t Size;
    typedef uint64 Index;
    typedef uint64 Time;

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

    // base texture formats, the upper 4 bits represent the number of channels
    typedef enum SR_TEXTURE_FORMAT: uint8 {
        SR_TEXTURE_FORMAT_R     = 0x10,
        SR_TEXTURE_FORMAT_RG    = 0x20,
        SR_TEXTURE_FORMAT_RGB   = 0x30,
        SR_TEXTURE_FORMAT_RGBA  = 0x40,
        SR_TEXTURE_FORMAT_BGR   = 0x31,
        SR_TEXTURE_FORMAT_BGRA  = 0x41
    } SR_TEXTURE_FORMAT;

    // Texture formats with data types, upper 4 bits represent the number of channels, lower 4 bits represent the data type
    typedef enum SR_TEXTURE_DATA_FORMAT: uint8 {
        // single channel
        SR_TEXTURE_DATA_FORMAT_R_UI8        = 0x12,
        SR_TEXTURE_DATA_FORMAT_R_I8         = 0x13,
        SR_TEXTURE_DATA_FORMAT_R_UI16       = 0x14,
        SR_TEXTURE_DATA_FORMAT_R_I16        = 0x15,
        SR_TEXTURE_DATA_FORMAT_R_UI32       = 0x16,
        SR_TEXTURE_DATA_FORMAT_R_I32        = 0x17,
        SR_TEXTURE_DATA_FORMAT_R_F16        = 0x1A,
        SR_TEXTURE_DATA_FORMAT_R_F32        = 0x1B,

        // two channels
        SR_TEXTURE_DATA_FORMAT_RG_UI8       = 0x22,
        SR_TEXTURE_DATA_FORMAT_RG_I8        = 0x23,
        SR_TEXTURE_DATA_FORMAT_RG_UI16      = 0x24,
        SR_TEXTURE_DATA_FORMAT_RG_I16       = 0x25,
        SR_TEXTURE_DATA_FORMAT_RG_UI32      = 0x26,
        SR_TEXTURE_DATA_FORMAT_RG_I32       = 0x27,
        SR_TEXTURE_DATA_FORMAT_RG_F16       = 0x2A,
        SR_TEXTURE_DATA_FORMAT_RG_F32       = 0x2B,

        // three channels
        SR_TEXTURE_DATA_FORMAT_RGB_UI8      = 0x32,
        SR_TEXTURE_DATA_FORMAT_RGB_I8       = 0x33,
        SR_TEXTURE_DATA_FORMAT_RGB_UI16     = 0x34,
        SR_TEXTURE_DATA_FORMAT_RGB_I16      = 0x35,
        SR_TEXTURE_DATA_FORMAT_RGB_UI32     = 0x36,
        SR_TEXTURE_DATA_FORMAT_RGB_I32      = 0x37,
        SR_TEXTURE_DATA_FORMAT_RGB_F16      = 0x3A,
        SR_TEXTURE_DATA_FORMAT_RGB_F32      = 0x3B,

        // four channels
        SR_TEXTURE_DATA_FORMAT_RGBA_UI8     = 0x42,
        SR_TEXTURE_DATA_FORMAT_RGBA_I8      = 0x43,
        SR_TEXTURE_DATA_FORMAT_RGBA_UI16    = 0x44,
        SR_TEXTURE_DATA_FORMAT_RGBA_I16     = 0x45,
        SR_TEXTURE_DATA_FORMAT_RGBA_UI32    = 0x46,
        SR_TEXTURE_DATA_FORMAT_RGBA_I32     = 0x47,
        SR_TEXTURE_DATA_FORMAT_RGBA_F16     = 0x4A,
        SR_TEXTURE_DATA_FORMAT_RGBA_F32     = 0x4B,

        // special formats
        SR_TEXTURE_DATA_FORMAT_DEPTH_16     = 0x51,
        SR_TEXTURE_DATA_FORMAT_DEPTH_24     = 0x52,
        SR_TEXTURE_DATA_FORMAT_DEPTH_32     = 0x53,
        SR_TEXTURE_DATA_FORMAT_DEPTH_24_STENCIL_8    = 0x54,
        SR_TEXTURE_DATA_FORMAT_DEPTH_32_STENCIL_8    = 0x55

    } SR_TEXTURE_DATA_FORMAT;

#if defined(SR_CORE_DEBUG)
    const static bool s_SyriusDebugMode = true;
#else
    const static bool s_SyriusDebugMode = false;
#endif

    /**
 * @brief This function will return the size of a type in bytes
 * @example SR_UINT8 -> 1 byte
 * @example SR_UINT16 -> 2 bytes
 * @example SR_UINT32 -> 4 bytes
 * @example SR_UINT64 -> 8 bytes
 * @param type The type to get the size of
 * @return the number of bytes needed to store data of that type
 */
    uint8 SR_CORE_API getTypeSize(SR_DATA_TYPE type);
    uint8 SR_CORE_API getTypeSize(SR_SCALAR_TYPE type);

    /**
     * @brief Returns the number of components in a scalar type
     * @example SR_FLOAT32_1 -> 1
     * @example SR_FLOAT64_1 -> 1
     * @example SR_UINT8_4 -> 4
     * @param type
     * @return the number of components in the scalar type
     */
    uint8 SR_CORE_API getScalarComponentCount(SR_SCALAR_TYPE type);

    SR_DATA_TYPE SR_CORE_API getScalarDataType(SR_SCALAR_TYPE type);

    SR_TEXTURE_FORMAT SR_CORE_API getTextureFormat(SR_TEXTURE_DATA_FORMAT format);

    uint8 SR_CORE_API getTextureChannelCount(SR_TEXTURE_FORMAT format);

    uint8 SR_CORE_API getTextureDataChannelCount(SR_TEXTURE_DATA_FORMAT format);

    SR_DATA_TYPE SR_CORE_API getTextureDataType(SR_TEXTURE_DATA_FORMAT format);

}

