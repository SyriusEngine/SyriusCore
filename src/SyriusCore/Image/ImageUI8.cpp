#include "ImageUI8.hpp"

#include "../Dependencies/stb_image/stb_image.hpp"
#include "../Dependencies/stb_image/stb_image_resize.hpp"
#include "../Dependencies/stb_image/stb_image_write.hpp"

#include "../Utils/DebugMacros.hpp"

namespace Syrius{


    ImageUI8::ImageUI8(const ImageFileDesc &desc):
    Image(),
    m_Data(){
        stbi_set_flip_vertically_on_load(desc.flipOnAccess);
        i32 channelCount, width, height;
        UByte* pData = stbi_load(desc.fileName.c_str(), &width, &height, &channelCount, 0);
        if (!pData){
            SR_LOG_WARNING("ImageUI8", "Image: %s failed to load", desc.fileName.c_str());
            return;
        }
        if (desc.requestedChannelCount == channelCount or desc.requestedChannelCount == 0){
            m_Data.resize(width * height * channelCount);
            memcpy(&m_Data[0], pData, width * height * channelCount);
        }
        else{
            m_Data.resize(width * height * desc.requestedChannelCount, static_cast<u8>(desc.defaultChannelValue));
            for (u32 i = 0; i < width * height; i++){
                for (u32 j = 0; j < desc.requestedChannelCount; j++){
                    m_Data[i * desc.requestedChannelCount + j] = pData[i * channelCount + j];
                }
            }
            channelCount = desc.requestedChannelCount;
        }
        m_Width = width;
        m_Height = height;
        m_Format = getFormatFromChannelCount(channelCount, SR_UINT8);

        stbi_image_free(pData);

        SR_POSTCONDITION(m_Width > 0, "[ImageUI8]: Image: %s has a width of 0", desc.fileName.c_str());
        SR_POSTCONDITION(m_Height > 0, "[ImageUI8]: Image: %s has a height of 0", desc.fileName.c_str());
    }

    ImageUI8::ImageUI8(const ImageUI8Desc &desc):
    Image(desc.width, desc.height, desc.format),
    m_Data(desc.width * desc.height * getTextureChannelCount(desc.format), desc.defaultChannelValue){
        SR_PRECONDITION(desc.width > 0, "[ImageUI8]: Image: %p cannot be created with a width of 0", this);
        SR_PRECONDITION(desc.height > 0, "[ImageUI8]: Image: %p cannot be created with a height of 0", this);
        SR_PRECONDITION(getTextureDataType(desc.format) == SR_UINT8, "[ImageUI8]: Image: %p cannot be created with a format (given = %i) that is not of type uint8", this, desc.format)

        if (desc.data != nullptr){
            memcpy(&m_Data[0], desc.data, desc.width * desc.height * getTextureChannelCount(desc.format));
        }
    }

    ImageUI8::~ImageUI8() = default;

    void ImageUI8::writeToFile(const ImageFileDesc &desc) const {
        stbi_flip_vertically_on_write(desc.flipOnAccess);
        auto width = static_cast<i32>(m_Width);
        auto height = static_cast<i32>(m_Height);
        i32 channelCount = getTextureChannelCount(m_Format);

        switch (desc.imgType) {
            case SR_IMAGE_PNG: {
                stbi_write_png(desc.fileName.c_str(), width, height, channelCount, &m_Data[0], width * channelCount);
                break;
            }
            case SR_IMAGE_JPG: {
                stbi_write_jpg(desc.fileName.c_str(), width, height, channelCount, &m_Data[0], 100); // last parameter is the quality, 100 is the highest quality
                break;
            }
            case SR_IMAGE_BMP: {
                stbi_write_bmp(desc.fileName.c_str(), width, height, channelCount, &m_Data[0]);
                break;
            }
            default: {
                SR_LOG_WARNING("ImageUI8", "Image: %s cannot be written to file type: %d", desc.fileName.c_str(), desc.imgType);
                break;
            }
        }
    }

    void ImageUI8::resize(u32 width, u32 height) {
        SR_PRECONDITION(width > 0, "[ImageUI8]: Image: %p cannot be resized to a width of 0", this);
        SR_PRECONDITION(height > 0, "[ImageUI8]: Image: %p cannot be resized to a height of 0", this);

        i32 channelCount = getTextureChannelCount(m_Format);
        std::vector<UByte> resizedData(width * height * channelCount);

        if (!stbir_resize_uint8(&m_Data[0], m_Width, m_Height, 0, &resizedData[0], width, height, 0, channelCount)){
            SR_LOG_WARNING("ImageUI8", "Image: %p failed to resize", this);
            return;
        }
        m_Data = resizedData;
        m_Width = width;
        m_Height = height;
    }

    void ImageUI8::extendAlpha() {
        u32 channelCount = getTextureChannelCount(m_Format);
        if (channelCount == 3){
            std::vector<UByte> rgbaData;
            addAlpha<UByte>(m_Data, rgbaData, m_Width, m_Height, channelCount);
            m_Data = rgbaData;
            m_Format = SR_TEXTURE_RGBA_UI8;
        }
    }

    void *ImageUI8::getData() {
        return m_Data.data();
    }

    UP<Image> ImageUI8::convertToUI8() {
        ImageUI8Desc desc;
        desc.data = m_Data.data();
        desc.width = m_Width;
        desc.height = m_Height;
        desc.format = m_Format;

        return UP<Image>(new ImageUI8(desc));
    }

}