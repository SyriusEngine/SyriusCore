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
        int32 channelCount, width, height;
        ubyte* pData = stbi_load(desc.fileName.c_str(), &width, &height, &channelCount, 0);
        if (!pData){
            SR_CORE_WARNING("[ImageUI8]: Image: %s failed to load", desc.fileName.c_str());
            return;
        }
        if (desc.requestedChannelCount == channelCount or desc.requestedChannelCount == 0){
            m_Data.resize(width * height * channelCount);
            memcpy(&m_Data[0], pData, width * height * channelCount);
        }
        else{
            m_Data.resize(width * height * desc.requestedChannelCount);
            for (uint32 i = 0; i < width * height; i++){
                for (uint32 j = 0; j < desc.requestedChannelCount; j++){
                    m_Data[i * desc.requestedChannelCount + j] = pData[i * channelCount + j];
                }
            }
            channelCount = desc.requestedChannelCount;
        }
        m_Width = width;
        m_Height = height;
        switch (channelCount) {
            case 1: m_Format = SR_TEXTURE_R_UI8; break;
            case 2: m_Format = SR_TEXTURE_RG_UI8; break;
            case 3: m_Format = SR_TEXTURE_RGB_UI8; break;
            case 4: m_Format = SR_TEXTURE_RGBA_UI8; break;
            default: SR_CORE_WARNING("[ImageUI8]: Image: %s has an unsupported number of channels: %d", desc.fileName.c_str(), channelCount);
        }

        stbi_image_free(pData);

        SR_CORE_POSTCONDITION(m_Width > 0, "[ImageUI8]: Image: %s has a width of 0", desc.fileName.c_str());
        SR_CORE_POSTCONDITION(m_Height > 0, "[ImageUI8]: Image: %s has a height of 0", desc.fileName.c_str());
    }

    ImageUI8::ImageUI8(const ImageUI8Desc &desc):
    Image(desc.width, desc.height, desc.format),
    m_Data(desc.width * desc.height * getTextureChannelCount(desc.format)){
        if (desc.data != nullptr){
            memcpy(&m_Data[0], desc.data, desc.width * desc.height * getTextureChannelCount(desc.format));
        }
    }

    ImageUI8::~ImageUI8() = default;

    void ImageUI8::writeToFile(const ImageFileDesc &desc) const {
        stbi_flip_vertically_on_write(desc.flipOnAccess);
        auto width = static_cast<int32>(m_Width);
        auto height = static_cast<int32>(m_Height);
        int32 channelCount = getTextureChannelCount(m_Format);

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
                SR_CORE_WARNING("[ImageUI8]: Image: %s cannot be written to file type: %d", desc.fileName.c_str(), desc.imgType);
                break;
            }
        }
    }

    void ImageUI8::resize(uint32 width, uint32 height) {
        int32 channelCount = getTextureChannelCount(m_Format);
        std::vector<ubyte> resizedData(width * height * channelCount);

        if (!stbir_resize_uint8(&m_Data[0], m_Width, m_Height, 0, &resizedData[0], width, height, 0, channelCount)){
            SR_CORE_WARNING("[ImageUI8]: Image: %x failed to resize", this);
            return;
        }
        m_Data = resizedData;
        m_Width = width;
        m_Height = height;
    }

    void ImageUI8::extendAlpha() {
        uint32 channelCount = getTextureChannelCount(m_Format);
        if (channelCount == 3){
            std::vector<ubyte> rgbaData;
            addAlpha<ubyte>(m_Data, rgbaData, m_Width, m_Height, channelCount);
            m_Data = rgbaData;
            m_Format = SR_TEXTURE_RGBA_UI8;
        }
    }

    void *ImageUI8::getData() {
        return m_Data.data();
    }

}