#include "ImageUI16.hpp"

#include "../Dependencies/stb_image/stb_image.hpp"
#include "../Dependencies/stb_image/stb_image_resize.hpp"
#include "../Dependencies/stb_image/stb_image_write.hpp"

#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ImageUI16::ImageUI16(const ImageFileDesc &desc):
    Image(),
    m_Data(){
        stbi_set_flip_vertically_on_load(desc.flipOnAccess);
        int32 width, height, channels;
        uint16* pData = stbi_load_16(desc.fileName.c_str(), &width, &height, &channels, desc.requestedChannelCount);
        if (!pData){
            SR_CORE_WARNING("[ImageUI16]: Image: %s failed to load", desc.fileName.c_str());
            return;
        }
        m_Data.resize(width * height * channels);
        memcpy(&m_Data[0], pData, width * height * channels);
        m_Width = width;
        m_Height = height;
        switch (channels) {
            case 1: m_Format = SR_TEXTURE_R_UI16; break;
            case 2: m_Format = SR_TEXTURE_RG_UI16; break;
            case 3: m_Format = SR_TEXTURE_RGB_UI16; break;
            case 4: m_Format = SR_TEXTURE_RGBA_UI16; break;
            default: SR_CORE_WARNING("[ImageUI16]: Image: %s has an unsupported number of channels: %d", desc.fileName.c_str(), channels);
        }

        stbi_image_free(pData);

        SR_CORE_POSTCONDITION(m_Width > 0, "[ImageUI16]: Image: %s has a width of 0", desc.fileName.c_str());
        SR_CORE_POSTCONDITION(m_Height > 0, "[ImageUI16]: Image: %s has a height of 0", desc.fileName.c_str());
    }

    ImageUI16::ImageUI16(const ImageUI16Desc &desc):
    Image(desc.width, desc.height, desc.format),
    m_Data(desc.width * desc.height * getTextureChannelCount(desc.format)){
        if (desc.data != nullptr){
            memcpy(&m_Data[0], desc.data, desc.width * desc.height * getTextureChannelCount(desc.format));
        }
    }

    ImageUI16::~ImageUI16() = default;

    void ImageUI16::writeToFile(const ImageFileDesc &desc) const {

    }

    void ImageUI16::resize(uint32 width, uint32 height) {
        int32 channelCount = getTextureChannelCount(m_Format);
        std::vector<uint16> resizedData(width * height * channelCount);

        if (!stbir_resize_uint16_generic(&m_Data[0], m_Width, m_Height, 0,
                                         &resizedData[0], width, height, 0,
                                         channelCount, -1,0,
                                         STBIR_EDGE_CLAMP, STBIR_FILTER_DEFAULT,
                                         STBIR_COLORSPACE_LINEAR, nullptr)){
            SR_CORE_WARNING("[ImageUI8]: Image: %x failed to resize", this);
            return;
        }
        m_Data = resizedData;
        m_Width = width;
        m_Height = height;

    }

    void ImageUI16::extendAlpha() {
        uint32 channelCount = getTextureChannelCount(m_Format);
        if (channelCount == 3){
            std::vector<uint16> rgbaData;
            addAlpha<uint16>(m_Data, rgbaData, m_Width, m_Height, channelCount);
            m_Data = rgbaData;
            m_Format = SR_TEXTURE_RGBA_UI16;
        }
    }

    void *ImageUI16::getData() {
        return m_Data.data();
    }
}

