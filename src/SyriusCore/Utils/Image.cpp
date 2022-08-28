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
            m_Data.resize(m_Width * m_Height * m_ChannelCount);
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

    int32 Image::getWidth() const {
        return m_Width;
    }

    int32 Image::getHeight() const {
        return m_Height;
    }

    int32 Image::getChannelCount() const {
        return m_ChannelCount;
    }

    const std::vector<ubyte> &Image::getData() const {
        return m_Data;
    }

    void Image::flipVertically() {
        std::vector<ubyte> flippedData(m_Data.size());
        for (int32 y = 0; y < m_Height; y++){
            for (int32 x = 0; x < m_Width; x++){
                for (int32 c = 0; c < m_ChannelCount; c++){
                    flippedData[(m_Height - y - 1) * m_Width * m_ChannelCount + x * m_ChannelCount + c] = m_Data[y * m_Width * m_ChannelCount + x * m_ChannelCount + c];
                }
            }
        }
        m_Data = flippedData;
    }

    void Image::extendAlpha(ubyte alpha) {
        SR_CORE_PRECONDITION(m_ChannelCount == 3, "Image::extendAlpha() can only be used on images with 3 channels")

        std::vector<ubyte> extendedData(m_Width * m_Height * 4);
        for (int32 i = 0; i < m_Width * m_Height; i += 3){
            extendedData[i] = m_Data[i];
            extendedData[i + 1] = m_Data[i + 1];
            extendedData[i + 2] = m_Data[i + 2];
            extendedData[i + 3] = alpha;
        }
        m_ChannelCount = 4;
        m_Data = extendedData;
    }

    void Image::runPixelFunc(pixelFunc func) {
        for (int32 y = 0; y < m_Height; y++){
            for (int32 x = 0; x < m_Width; x++){
                func(&m_Data[(y * m_Width + x) * m_ChannelCount], x, y, m_ChannelCount);
            }
        }

    }

}