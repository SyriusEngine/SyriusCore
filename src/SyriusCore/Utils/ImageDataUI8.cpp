#include "ImageDataUI8.hpp"

namespace Syrius{

    ImageDataUI8::ImageDataUI8(const std::string &fileName, bool flipOnLoad):
    ImageData(0, 0, 0, SR_TEXTURE_RGBA_UI8) {
        stbi_set_flip_vertically_on_load(flipOnLoad);
        ubyte* data = stbi_load(fileName.c_str(), &m_Width, &m_Height, &m_ChannelCount, 0);
        if (data){
            m_Data.resize(m_Width * m_Height * m_ChannelCount);
            memcpy(&m_Data[0], data, m_Width * m_Height * m_ChannelCount);
            switch(m_ChannelCount){
                case 1: m_Format = SR_TEXTURE_R_UI8; break;
                case 2: m_Format = SR_TEXTURE_RG_UI8; break;
                case 3: m_Format = SR_TEXTURE_RGB_UI8; break;
                case 4: m_Format = SR_TEXTURE_RGBA_UI8; break;
                default: SR_CORE_WARNING("Image: %s has an unsupported number of channels: %d", fileName.c_str(), m_ChannelCount);
            }
        }
        else{
            SR_CORE_WARNING("Image: %s failed to load", fileName.c_str());
        }
        stbi_image_free(data);
    }

    ImageDataUI8::ImageDataUI8(const ubyte *pixelData, int32 width, int32 height, SR_TEXTURE_FORMAT format):
    ImageData(width, height, getTextureDataChannelCount(format), format){
        m_Data.reserve(m_Width * m_Height * m_ChannelCount);
        memcpy(&m_Data[0], pixelData, m_Width * m_Height * m_ChannelCount);
    }

    ImageDataUI8::~ImageDataUI8() = default;

    void ImageDataUI8::writeToFile(const std::string &fileName, bool flipOnWrite, SR_IMAGE_TYPE imgType) const {
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
            case SR_IMAGE_HDR: {
                auto width = m_Width;
                auto height = m_Height;
                auto channelCount = m_ChannelCount;
                float* data = stbi_loadf_from_memory(&m_Data[0], m_Data.size(), &width, &height, &channelCount, 0);
                stbi_write_hdr(fileName.c_str(), width, height, channelCount, data);
                stbi_image_free(data);
            }
            default: {
                SR_CORE_WARNING("Image: %s failed to write: Unknown file format", fileName.c_str());
                break;
            }
        }
    }

    void ImageDataUI8::resize(int32 newWidth, int32 newHeight) {
        auto* buf = new ubyte[newWidth * newHeight * m_ChannelCount];
        if (stbir_resize_uint8(&m_Data[0], m_Width, m_Height, 0, buf, newWidth, newHeight, 0, m_ChannelCount)){
            m_Data.resize(newWidth * newHeight * m_ChannelCount);
            memcpy(&m_Data[0], buf, newWidth * newHeight * m_ChannelCount);
            m_Width = newWidth;
            m_Height = newHeight;
        }
        else{
            SR_CORE_WARNING("Image: failed to resize (%d, %d) -> (%d, %d)", m_Width, m_Height, newWidth, newHeight);
        }
        delete[] buf;
    }

    const void *ImageDataUI8::getData() const {
        return m_Data.data();
    }

    void ImageDataUI8::extendAlpha(ubyte alpha) {
        ::Syrius::extendAlpha<ubyte>(m_Data, m_Width, m_Height, m_ChannelCount, alpha);

    }
}
