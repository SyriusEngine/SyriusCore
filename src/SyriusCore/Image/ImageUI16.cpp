#include "ImageUI16.hpp"

#include "../Dependencies/stb_image/stb_image.hpp"
#include "../Dependencies/stb_image/stb_image_resize.hpp"
#include "../Dependencies/stb_image/stb_image_write.hpp"

#include "../Utils/DebugMacros.hpp"
#include "ImageUI8.hpp"

namespace Syrius{

    ImageUI16::ImageUI16(const ImageFileDesc &desc):
    Image(),
    m_Data(){
        stbi_set_flip_vertically_on_load(desc.flipOnAccess);
        i32 width, height, channels;
        u16* pData = stbi_load_16(desc.fileName.string().c_str(), &width, &height, &channels, desc.requestedChannelCount);
        if (!pData){
            SR_LOG_WARNING("ImageUI16", "Image {} failed to load", desc.fileName.string());
            return;
        }
        m_Data.resize(width * height * channels);
        memcpy(&m_Data[0], pData, width * height * channels);
        m_Width = width;
        m_Height = height;
        m_Format = getFormatFromChannelCount(channels, SR_UINT16);

        stbi_image_free(pData);

        SR_POSTCONDITION(m_Width > 0, "[ImageUI16]: Image: {} has a width of 0", desc.fileName.string());
        SR_POSTCONDITION(m_Height > 0, "[ImageUI16]: Image: {} has a height of 0", desc.fileName.string());
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

    void ImageUI16::resize(u32 width, u32 height) {
        i32 channelCount = getTextureChannelCount(m_Format);
        std::vector<u16> resizedData(width * height * channelCount);

        if (!stbir_resize_uint16_generic(&m_Data[0], m_Width, m_Height, 0,
                                         &resizedData[0], width, height, 0,
                                         channelCount, -1,0,
                                         STBIR_EDGE_CLAMP, STBIR_FILTER_DEFAULT,
                                         STBIR_COLORSPACE_LINEAR, nullptr)){
            SR_LOG_WARNING("ImageUI16", "Image failed to resize");
            return;
        }
        m_Data = resizedData;
        m_Width = width;
        m_Height = height;

    }

    void ImageUI16::extendAlpha() {
        u32 channelCount = getTextureChannelCount(m_Format);
        if (channelCount == 3){
            std::vector<u16> rgbaData;
            addAlpha<u16>(m_Data, rgbaData, m_Width, m_Height, channelCount);
            m_Data = rgbaData;
            m_Format = SR_TEXTURE_RGBA_UI16;
        }
    }

    void *ImageUI16::getData() {
        return m_Data.data();
    }

    UP<Image> ImageUI16::convertToUI8() {
        ImageUI8Desc desc;
        desc.width = m_Width;
        desc.height = m_Height;
        desc.format = getFormatFromChannelCount(getTextureChannelCount(m_Format), SR_UINT8);

        auto img = createUP<ImageUI8>(desc);
        auto ui8Data = img->getData();
        auto channelCount = getTextureChannelCount(desc.format);
        for (u32 i = 0; i < m_Width * m_Height; i++){
            for (u32 j = 0; j < channelCount; j++){
                reinterpret_cast<UByte*>(ui8Data)[i * channelCount + j] = static_cast<UByte>(m_Data[i * channelCount + j] / 256);
            }
        }

        return img;
    }
}

