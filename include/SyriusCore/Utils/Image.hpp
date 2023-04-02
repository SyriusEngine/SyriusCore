#pragma once

#include "../Core/SyriusCoreInclude.hpp"
#include "Resource.hpp"

namespace Syrius{

    typedef enum SR_IMAGE_TYPE {
        SR_IMAGE_PNG    = 0x01,
        SR_IMAGE_JPG    = 0x02,
        SR_IMAGE_BMP    = 0x03,
        SR_IMAGE_HDR    = 0x04,
    } SR_IMAGE_TYPE;

    class ImageData;

    class SR_CORE_API Image{
    private:
        Resource<ImageData> m_ImageData;

    public:
        Image(const std::string& fileName, bool flipOnLoad);

        Image(const ubyte* pixelData, int32 width, int32 height, SR_TEXTURE_DATA_FORMAT format);

        ~Image();

        void writeToFile(const std::string& fileName, bool flipOnWrite, SR_IMAGE_TYPE imgType = SR_IMAGE_PNG) const;

        void resize(int32 newWidth, int32 newHeight);

        [[nodiscard]] int32 getWidth() const;

        [[nodiscard]] int32 getHeight() const;

        [[nodiscard]] int32 getChannelCount() const;

        [[nodiscard]] const void* getData() const;

        void extendAlpha(ubyte alpha = 255);

    };

}