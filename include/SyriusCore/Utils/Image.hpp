#pragma once

#include "../Core/SyriusCoreInclude.hpp"

namespace Syrius{

    typedef enum SR_IMAGE_TYPE {
        SR_IMAGE_PNG    = 0x01,
        SR_IMAGE_JPG    = 0x02,
        SR_IMAGE_BMP    = 0x03
    } SR_IMAGE_TYPE;

    typedef ubyte(*pixelFunc)(ubyte* pixel, int32 x, int32 y, int32 comp);

    class SR_CORE_API Image{
    private:
        int32 m_Width;
        int32 m_Height;
        int32 m_ChannelCount;

        std::vector<ubyte> m_Data;

    public:
        Image(const std::string& fileName, bool flipOnLoad);

        Image(const ubyte* pixelData, int32 width, int32 height, int32 channelCount);

        ~Image();

        void writeToFile(const std::string& fileName, bool flipOnWrite, SR_IMAGE_TYPE imgType = SR_IMAGE_PNG);

        void resize(int32 newWidth, int32 newHeight);

        [[nodiscard]] int32 getWidth() const;

        [[nodiscard]] int32 getHeight() const;

        [[nodiscard]] int32 getChannelCount() const;

        [[nodiscard]] const std::vector<ubyte>& getData() const;

        void flipVertically();

        void extendAlpha(ubyte alpha = 255);

        void runPixelFunc(pixelFunc func);

    };

}
