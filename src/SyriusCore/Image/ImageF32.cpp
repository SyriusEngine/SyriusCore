#include "ImageF32.hpp"

#include "../Dependencies/stb_image/stb_image.hpp"
#include "../Dependencies/stb_image/stb_image_resize.hpp"
#include "../Dependencies/stb_image/stb_image_write.hpp"

#include "../Utils/DebugMacros.hpp"
#include "ImageUI8.hpp"

namespace Syrius{

    ImageF32::ImageF32(const ImageFileDesc &desc):
    Image(),
    m_Data(){
        SR_PRECONDITION(stbi_is_hdr(desc.fileName.string().c_str()), "[ImageF32]: Image: %s is not a HDR image", desc.fileName.c_str());

        stbi_set_flip_vertically_on_load(desc.flipOnAccess);
        i32 channelCount, width, height;
        float* pData = stbi_loadf(desc.fileName.string().c_str(), &width, &height, &channelCount, 0);
        if (!pData){
            SR_LOG_WARNING("ImageF32", "Image: %s failed to load", desc.fileName.c_str());
            return;
        }
        if (desc.requestedChannelCount == channelCount or desc.requestedChannelCount == 0){
            m_Data.resize(width * height * channelCount);
            memcpy(&m_Data[0], pData, width * height * channelCount);
        }
        else{
            m_Data.resize(width * height * desc.requestedChannelCount, static_cast<float>(desc.defaultChannelValue));
            for (u32 i = 0; i < width * height; i++){
                for (u32 j = 0; j < desc.requestedChannelCount; j++){
                    m_Data[i * desc.requestedChannelCount + j] = pData[i * channelCount + j];
                }
            }
            channelCount = desc.requestedChannelCount;
        }
        m_Width = width;
        m_Height = height;
        m_Format = getFormatFromChannelCount(channelCount, SR_FLOAT32);

        stbi_image_free(pData);
    }

    ImageF32::ImageF32(const ImageF32Desc &desc):
    Image(desc.width, desc.height, desc.format),
    m_Data(desc.width * desc.height * getTextureChannelCount(desc.format), desc.defaultChannelValue){
        SR_PRECONDITION(desc.width > 0, "[ImageF32]: Image: %p cannot be created with a width of 0", this);
        SR_PRECONDITION(desc.height > 0, "[ImageF32]: Image: %p cannot be created with a height of 0", this);
        SR_PRECONDITION(getTextureDataType(desc.format) == SR_FLOAT32, "[ImageF32]: Image: %p cannot be created with a format (given = %i) that is not of type float", this, desc.format)

        if (desc.data != nullptr){
            memcpy(&m_Data[0], desc.data, desc.width * desc.height * getTextureChannelCount(desc.format));
        }
    }

    ImageF32::~ImageF32() = default;

    void ImageF32::writeToFile(const ImageFileDesc &desc) const {
        stbi_flip_vertically_on_write(desc.flipOnAccess);
        auto width = static_cast<i32>(m_Width);
        auto height = static_cast<i32>(m_Height);
        i32 channelCount = getTextureChannelCount(m_Format);
        switch (desc.imgType){
            case SR_IMAGE_HDR: {
                if (!stbi_write_hdr(desc.fileName.string().c_str(), width, height, channelCount, &m_Data[0])){
                    SR_LOG_WARNING("ImageF32", "Image: %s failed to write", desc.fileName.c_str());
                }
                break;
            }
            default: {
                SR_LOG_WARNING("ImageF32", "Image: %s has an unsupported image type: %d", desc.fileName.c_str(), desc.imgType);
                break;
            }
        }
    }

    void ImageF32::resize(u32 width, u32 height) {
        SR_PRECONDITION(width > 0, "[ImageF32]: Image: %p cannot be resized to a width of 0", this);
        SR_PRECONDITION(height > 0, "[ImageF32]: Image: %p cannot be resized to a height of 0", this);

        i32 channelCount = getTextureChannelCount(m_Format);
        std::vector<float> resizedData(width * height * channelCount);

        if (!stbir_resize_float(&m_Data[0], m_Width, m_Height, 0, &resizedData[0], width, height, 0, channelCount)){
            SR_LOG_WARNING("ImageF32", "Image: %p failed to resize", this);
            return;
        }
        m_Data = resizedData;
        m_Width = width;
        m_Height = height;
    }

    void ImageF32::extendAlpha() {
        u32 channelCount = getTextureChannelCount(m_Format);
        if (channelCount == 3){
            std::vector<float> rgbaData;
            addAlpha<float>(m_Data, rgbaData, m_Width, m_Height, channelCount);
            m_Data = rgbaData;
            m_Format = SR_TEXTURE_RGBA_F32;
        }
    }

    void *ImageF32::getData() {
        return m_Data.data();
    }

    UP<Image> ImageF32::convertToUI8() {
        ImageUI8Desc desc;
        desc.width = m_Width;
        desc.height = m_Height;
        desc.format = getFormatFromChannelCount(getTextureChannelCount(m_Format), SR_UINT8);

        auto img = createUP<ImageUI8>(desc);
        auto ui8Data = img->getData();
        auto channelCount = getTextureChannelCount(desc.format);
        for (u32 i = 0; i < m_Width * m_Height; i++){
            for (u32 j = 0; j < channelCount; j++){
                reinterpret_cast<UByte*>(ui8Data)[i * channelCount + j] = static_cast<UByte>(m_Data[i * channelCount + j] * 255.0f);
            }
        }

        return img;
    }
}
