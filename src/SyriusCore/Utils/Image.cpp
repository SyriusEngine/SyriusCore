#include "../../../../include/SyriusCore/Utils/Image.hpp"

#include "../Dependencies/stb_image/stb_image.hpp"
#include "../Dependencies/stb_image/stb_image_resize.hpp"
#include "../Dependencies/stb_image/stb_image_write.hpp"

namespace Syrius{

    Image::Image(const std::string &fileName, bool flipOnLoad)
            : m_ChannelCount(0),
              m_Height(0),
              m_Width(0){
        stbi_set_flip_vertically_on_load(flipOnLoad);
        auto pixelData = stbi_load(fileName.c_str(), &m_Width, &m_Height, &m_ChannelCount, 0);
        if (pixelData){
            m_Data.reserve(m_Width * m_Height * m_ChannelCount);
            memcpy(&m_Data[0], pixelData, m_Width * m_Height * m_ChannelCount);

            // we copied the memory contents in our own buffer, we do not need this pointer anymore
            stbi_image_free(pixelData);
        }
        else{
            SR_CORE_WARNING("Image: " + fileName + " failed to load")
        }
    }

    Image::Image(const ubyte *pixelData, int32 width, int32 height, int32 channelCount)
            : m_ChannelCount(channelCount),
              m_Width(width),
              m_Height(height){
        m_Data.reserve(m_Width * m_Height * m_ChannelCount);
        memcpy(&m_Data[0], pixelData, m_Width * m_Height * m_ChannelCount);
    }

    Image::Image(const std::vector<ubyte> &pixelData, int32 width, int32 height, int32 channelCount)
            : m_ChannelCount(channelCount),
              m_Width(width),
              m_Height(height),
              m_Data(pixelData){

    }

    Image::~Image() {
        m_Data.clear();
    }

    void Image::writeToFile(const std::string &fileName, bool flipOnWrite, SR_IMAGE_TYPE imgType) {
        stbi_flip_vertically_on_write(flipOnWrite);
        switch (imgType){
            case SR_IMAGE_PNG: {
                stbi_write_png(fileName.c_str(), m_Width, m_Height, m_ChannelCount, &m_Data[0], m_Width * m_ChannelCount);
                break;
            }
            case SR_IMAGE_BMP: {
                stbi_write_bmp(fileName.c_str(), m_Width, m_Height, m_ChannelCount, &m_Data[0]);
                break;
            }
            case SR_IMAGE_JPG: {
                stbi_write_jpg(fileName.c_str(), m_Width, m_Height, m_ChannelCount, &m_Data[0], 100); // 100 indicates no compression (= no loss of quality)
                break;
            }
        }
    }

    void Image::resize(int32 newWidth, int32 newHeight) {
        auto* buf = new ubyte[newWidth * newHeight * m_ChannelCount];
        if (stbir_resize_uint8(&m_Data[0], m_Width, m_Height, 0, buf, newWidth, newHeight, 0, m_ChannelCount)){
            m_Data.clear();
            m_Data.reserve(newWidth * newHeight * m_ChannelCount);
            memcpy(&m_Data[0], buf, newWidth * newHeight * m_ChannelCount);
            m_Width = newWidth;
            m_Height = newHeight;
        }
        delete[] buf;
    }

    void Image::addAlphaChannel() {
        // only extend the channel count to 4 if the data only has 3 channels
        if (m_ChannelCount == 3){
            std::vector<uint8> buf(m_Width * m_Height * 4);
            auto size = m_Width * m_Height * m_ChannelCount;
            uint32 bufIndex = 0;
            for (uint32 i = 0; i < size; i += 3){
                buf[bufIndex] = m_Data[i];
                bufIndex++;
                buf[bufIndex] = m_Data[i + 1];
                bufIndex++;
                buf[bufIndex] = m_Data[i + 2];
                bufIndex++;
                buf[bufIndex] = 255;
                bufIndex++;
            }
            m_Data.clear();
            m_Data = buf;
            m_ChannelCount = 4;
        }
    }

    void Image::makeTransparent(uint8 red, uint8 green, uint8 blue) {
        addAlphaChannel(); // just in case the data does not have an alpha channel, we are going to need it

        auto size = m_Width * m_Height * m_ChannelCount;
        for (uint32 i = 0; i < size; i += 4){
            if (m_Data[i] == red and m_Data[i + 1] == green and m_Data[i + 2] == blue){
                m_Data[i + 3] = 0;
            }
        }
    }

    void Image::invert() {
        auto size = m_Width * m_Height * m_ChannelCount;
        // small optimization possible for images with less than 4 channels
        if (m_ChannelCount == 4){
            for (uint32 i = 0; i < size; i += 4){
                m_Data[i] = 255 - m_Data[i];
                m_Data[i + 1] = 255 - m_Data[i + 1];
                m_Data[i + 2] = 255 - m_Data[i + 2];
            }
        }
        else{
            for (uint32 i = 0; i < size; i++){
                m_Data[i] = 255 - m_Data[i];
            }
        }
    }

    void Image::setContrast(int32 c) {
        int32 f = (259 * (c + 255)) / (255 * (259 - c));
        auto clamp = [](int32 x) -> uint8 {
            if (x > 255){
                return 255;
            }
            else if (x < 0){
                return 0;
            }
            else{
                return x;
            }
        };
        auto size = m_Width * m_Height * m_ChannelCount;
        // same optimization as other functions
        if (m_ChannelCount == 4){
            for (uint32 i = 0; i < size; i += 4){
                m_Data[i] = clamp(f * (m_Data[i] - 128) + 128);
                m_Data[i + 1] = clamp(f * (m_Data[i + 1] - 128) + 128);
                m_Data[i + 2] = clamp(f * (m_Data[i + 2] - 128) + 128);
            }
        }
        else{
            for (uint32 i = 0; i < size; i++){
                m_Data[i] = clamp(f * (m_Data[i] - 128) + 128);
            }
        }
    }

    void Image::setBrightness(int32 brightness) {
        auto clamp = [](int32 x) -> uint8 {
            if (x > 255){
                return 255;
            }
            else if (x < 0){
                return 0;
            }
            else{
                return x;
            }
        };
        auto size = m_Width * m_Height * m_ChannelCount;
        if (m_ChannelCount == 4){
            for (uint32 i = 0; i < size; i += 4){
                m_Data[i] = clamp(m_Data[i] + brightness);
                m_Data[i + 1] = clamp(m_Data[i + 1] + brightness);
                m_Data[i + 2] = clamp(m_Data[i + 2] + brightness);
            }
        }
        else{
            for (uint32 i = 0; i < size; i++){
                m_Data[i] = clamp(m_Data[i] + brightness);
            }
        }
    }

    void Image::setPixelColor(uint32 x, uint32 y, uint8 red, uint8 green, uint8 blue, uint8 alpha) {
        std::vector<uint8> color = {red, green, blue, alpha};
        uint32 addr = ((m_Width * y) + x) * m_ChannelCount;
        for (uint32 i = 0; i < m_ChannelCount; i++){
            m_Data[addr + i] = color[i];
        }
    }

    std::vector<ubyte> Image::getPixelColor(uint32 x, uint32 y) {
        std::vector<uint8> color(m_ChannelCount);
        uint32 addr = ((m_Width * y) + x) * m_ChannelCount;
        for (uint32 i = 0; i < m_ChannelCount; i++){
            color[i] = m_Data[addr + i];
        }
        return color;
    }

    int32 Image::getWidth() const {
        return m_Width;
    }

    int32 Image::getHeight() const {
        return m_Height;
    }

    int32 Image::getChannelCount() const {
        return m_ChannelCount;
    }

    int32 Image::getSize() const {
        return m_Width * m_Height * m_ChannelCount;
    }

    std::vector<ubyte> Image::getPixelsBGRA() {
        if (m_ChannelCount < 3){
            return {};
        }
        if (m_ChannelCount == 3){
            addAlphaChannel();
        }
        auto size = m_Width * m_Height * m_ChannelCount;
        std::vector<ubyte> retVal(size);
        for (uint32 i = 0; i < size; i += m_ChannelCount){
            retVal[i] = m_Data[i + 2];
            retVal[i + 1] = m_Data[i + 1];
            retVal[i + 2] = m_Data[i];
            retVal[i + 3] = m_Data[i + 3];
        }
        return retVal;

    }

    const std::vector<ubyte>& Image::getPixels() const {
        return m_Data;
    }

}