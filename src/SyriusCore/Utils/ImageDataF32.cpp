#include "ImageDataF32.hpp"

namespace Syrius{

    ImageDataF32::ImageDataF32(const std::string &fileName, bool flipOnLoad):
    ImageData(0, 0, 0, SR_TEXTURE_DATA_RGBA_F32){
        stbi_set_flip_vertically_on_load(flipOnLoad);
        float* data = stbi_loadf(fileName.c_str(), &m_Width, &m_Height, &m_ChannelCount, 0);
        if (data){
            m_Data.resize(m_Width * m_Height * m_ChannelCount);
            memcpy(&m_Data[0], data, m_Width * m_Height * m_ChannelCount);
            switch(m_ChannelCount){
                case 1: m_Format = SR_TEXTURE_DATA_R_F32; break;
                case 2: m_Format = SR_TEXTURE_DATA_RG_F32; break;
                case 3: m_Format = SR_TEXTURE_DATA_RGB_F32; break;
                case 4: m_Format = SR_TEXTURE_DATA_RGBA_F32; break;
                default: SR_CORE_WARNING("Image: %s has an unsupported number of channels: %d", fileName.c_str(), m_ChannelCount);
            }
        }
        stbi_image_free(data);
    }

    ImageDataF32::ImageDataF32(const float *pixelData, int32 width, int32 height, SR_TEXTURE_DATA_FORMAT format):
    ImageData(width, height, getTextureDataChannelCount(format), format){
        m_Data.reserve(m_Width * m_Height * m_ChannelCount);
        memcpy(&m_Data[0], pixelData, m_Width * m_Height * m_ChannelCount);
    }

    ImageDataF32::~ImageDataF32() = default;

    void ImageDataF32::writeToFile(const std::string &fileName, bool flipOnWrite, SR_IMAGE_TYPE imgType) const {
        stbi_flip_vertically_on_write(flipOnWrite);
        switch (imgType) {
            case SR_IMAGE_HDR: {
                stbi_write_hdr(fileName.c_str(), m_Width, m_Height, m_ChannelCount, &m_Data[0]);
                break;
            }
            default: {
                SR_CORE_WARNING("Image: %s failed to write: Unknown file format", fileName.c_str());
                break;
            }
        }
    }

    void ImageDataF32::resize(int32 newWidth, int32 newHeight) {
        auto* buf = new float[newWidth * newHeight * m_ChannelCount];
        if (stbir_resize_float(&m_Data[0], m_Width, m_Height, 0, buf, newWidth, newHeight, 0, m_ChannelCount)){
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

    const void *ImageDataF32::getData() const {
        return m_Data.data();
    }

    void ImageDataF32::extendAlpha(ubyte alpha) {
        ::Syrius::extendAlpha<float>(m_Data, m_Width, m_Height, m_ChannelCount, alpha);
    }
}