#include "ImageF32.hpp"

#include "../Dependencies/stb_image/stb_image.hpp"
#include "../Dependencies/stb_image/stb_image_resize.hpp"
#include "../Dependencies/stb_image/stb_image_write.hpp"

#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ImageF32::ImageF32(const ImageFileDesc &desc):
    Image(),
    m_Data(){
        SR_CORE_PRECONDITION(stbi_is_hdr(desc.fileName.c_str()), "[ImageF32]: Image: %s is not a HDR image", desc.fileName.c_str());
        int32 channelCount, width, height;
        float* pData = stbi_loadf(desc.fileName.c_str(), &width, &height, &channelCount, desc.requestedChannelCount);
        if (!pData){
            SR_CORE_WARNING("[ImageF32]: Image: %s failed to load", desc.fileName.c_str());
            return;
        }
        m_Data.resize(width * height * channelCount);
        memcpy(&m_Data[0], pData, width * height * channelCount);
        m_Width = width;
        m_Height = height;
        switch (channelCount) {
            case 1: m_Format = SR_TEXTURE_R_F32; break;
            case 2: m_Format = SR_TEXTURE_RG_F32; break;
            case 3: m_Format = SR_TEXTURE_RGB_F32; break;
            case 4: m_Format = SR_TEXTURE_RGBA_F32; break;
            default: SR_CORE_WARNING("[ImageF32]: Image: %s has an unsupported number of channels: %d", desc.fileName.c_str(), channelCount);
        }
        stbi_image_free(pData);
    }

    ImageF32::ImageF32(const ImageF32Desc &desc):
    Image(desc.width, desc.height, desc.format),
    m_Data(desc.width * desc.height * getTextureFormatChannelCount(desc.format)){
        memcpy(&m_Data[0], desc.data, desc.width * desc.height * getTextureFormatChannelCount(desc.format));
    }

    ImageF32::~ImageF32() {

    }

    void ImageF32::writeToFile(const ImageFileDesc &desc) const {
        stbi_flip_vertically_on_write(desc.flipOnAccess);
        auto width = static_cast<int32>(m_Width);
        auto height = static_cast<int32>(m_Height);
        int32 channelCount = getTextureFormatChannelCount(m_Format);
        switch (desc.imgType){
            case SR_IMAGE_HDR: {
                if (!stbi_write_hdr(desc.fileName.c_str(), width, height, channelCount, &m_Data[0])){
                    SR_CORE_WARNING("[ImageF32]: Image: %s failed to write", desc.fileName.c_str());
                }
                break;
            }
            default: {
                SR_CORE_WARNING("[ImageF32]: Image: %s has an unsupported image type: %d", desc.fileName.c_str(), desc.imgType);
                break;
            }
        }
    }

    void ImageF32::resize(uint32 width, uint32 height) {
        int32 channelCount = getTextureFormatChannelCount(m_Format);
        std::vector<float> resizedData(width * height * channelCount);

        if (!stbir_resize_float(&m_Data[0], m_Width, m_Height, 0, &resizedData[0], width, height, 0, channelCount)){
            SR_CORE_WARNING("[ImageF32]: Image: %x failed to resize", this);
            return;
        }
        m_Data = resizedData;
        m_Width = width;
        m_Height = height;
    }

    void ImageF32::extendAlpha() {
        uint32 channelCount = getTextureFormatChannelCount(m_Format);
        if (channelCount == 3){
            std::vector<float> rgbaData;
            addAlpha<float>(m_Data, rgbaData, m_Width, m_Height, channelCount);
            m_Data = rgbaData;
            m_Format = SR_TEXTURE_RGBA_F32;
        }
    }

    const void *ImageF32::getData() const {
        return reinterpret_cast<const void*>(m_Data.data());
    }

}
