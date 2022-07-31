#pragma once

#include "../Core/SyriusCoreInclude.hpp"
#include "../../../src/SyriusCore/Core/DebugMacros.hpp"

namespace Syrius{

    typedef enum SR_IMAGE_TYPE {
        SR_IMAGE_PNG    = 0x01,
        SR_IMAGE_JPG    = 0x02,
        SR_IMAGE_BMP    = 0x03
    } SR_IMAGE_TYPE;

    class SR_API Image{
    private:
        int32 m_Width;
        int32 m_Height;
        int32 m_ChannelCount;

        std::vector<ubyte> m_Data;

    public:
        Image(const std::string& fileName, bool flipOnLoad);

        Image(const ubyte* pixelData, int32 width, int32 height, int32 channelCount);

        Image(const std::vector<ubyte>& pixelData, int32 width, int32 height, int32 channelCount);

        ~Image();

        void writeToFile(const std::string& fileName, bool flipOnWrite, SR_IMAGE_TYPE imgType = SR_IMAGE_PNG);

        void resize(int32 newWidth, int32 newHeight);

        void addAlphaChannel();

        void makeTransparent(uint8 red, uint8 green, uint8 blue);

        void invert();

        void setContrast(int32 c);

        void setBrightness(int32 brightness);

        void setPixelColor(uint32 x, uint32 y, uint8 red, uint8 green, uint8 blue, uint8 alpha = 255);

        std::vector<ubyte> getPixelColor(uint32 x, uint32 y);

        [[nodiscard]] int32 getWidth() const;

        [[nodiscard]] int32 getHeight() const;

        [[nodiscard]] int32 getChannelCount() const;

        [[nodiscard]] int32 getSize() const;

        [[nodiscard]] std::vector<ubyte> getPixelsBGRA();

        [[nodiscard]] const std::vector<ubyte>& getPixels() const;

    };

}
