#pragma once

#include "../Utils/SyriusCoreInclude.hpp"
#include <SyriusUtils/SyriusUtils.hpp>

typedef enum SR_IMAGE_TYPE {
    SR_IMAGE_PNG    = 0x01,
    SR_IMAGE_JPG    = 0x02,
    SR_IMAGE_BMP    = 0x03,
    SR_IMAGE_HDR    = 0x04,
} SR_IMAGE_TYPE;

namespace Syrius{

    struct SR_CORE_API ImageFileDesc{
        fs::path fileName;
        bool flipOnAccess               = true;
        SR_IMAGE_TYPE imgType           = SR_IMAGE_PNG;
        u32 requestedChannelCount    = 0; // 0 => all channels available in the image
        u64 defaultChannelValue      = 255; // if the image has fewer channels than requested, fill the rest with this value
    };

    struct SR_CORE_API ImageDesc{
        void* data                   = nullptr;
        u32 width                  = 0;
        u32 height                 = 0;
        SR_TEXTURE_FORMAT format     = SR_TEXTURE_NONE;
    };

    struct SR_CORE_API ImageUI8Desc{
        const UByte* data           = nullptr;
        u32 width                 = 0;
        u32 height                = 0;
        SR_TEXTURE_FORMAT format    = SR_TEXTURE_RGBA_UI8;
        u8 defaultChannelValue   = 255;
    };

    struct SR_CORE_API ImageUI16Desc{
        const u16* data          = nullptr;
        u32 width                 = 0;
        u32 height                = 0;
        SR_TEXTURE_FORMAT format    = SR_TEXTURE_RGBA_UI16;
        u16 defaultChannelValue  = 65535;

    };

    struct SR_CORE_API ImageF32Desc{
        const float* data           = nullptr;
        u32 width                 = 0;
        u32 height                = 0;
        SR_TEXTURE_FORMAT format    = SR_TEXTURE_RGBA_F32;
        float defaultChannelValue   = 1.0f;
    };
}

template<>
struct fmt::formatter<SR_IMAGE_TYPE> : fmt::ostream_formatter {
};

