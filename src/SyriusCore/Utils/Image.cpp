#include "../../../../include/SyriusCore/Utils/Image.hpp"
#include "ImageDataF32.hpp"
#include "ImageDataUI8.hpp"

#include "../Dependencies/stb_image/stb_image.hpp"
#include "../Dependencies/stb_image/stb_image_resize.hpp"
#include "../Dependencies/stb_image/stb_image_write.hpp"

namespace Syrius{

    Image::Image(const std::string &fileName, bool flipOnLoad){
        auto isHdr = stbi_is_hdr(fileName.c_str());
        printf("isHdr: %d", isHdr);
        if (stbi_is_hdr(fileName.c_str())){
            m_ImageData = Resource<ImageData>(new ImageDataF32(fileName, flipOnLoad));
        }
        else{
            m_ImageData = Resource<ImageData>(new ImageDataUI8(fileName, flipOnLoad));
        }
    }

    Image::Image(const ubyte *pixelData, int32 width, int32 height, SR_TEXTURE_DATA_FORMAT format){
        m_ImageData = Resource<ImageData>(new ImageDataUI8(pixelData, width, height, format));
    }

    Image::~Image() = default;

    void Image::writeToFile(const std::string &fileName, bool flipOnWrite, SR_IMAGE_TYPE imgType) const {
        m_ImageData->writeToFile(fileName, flipOnWrite, imgType);
    }

    void Image::resize(int32 newWidth, int32 newHeight) {
        m_ImageData->resize(newWidth, newHeight);
    }

    int32 Image::getWidth() const {
        return m_ImageData->m_Width;
    }

    int32 Image::getHeight() const {
        return m_ImageData->m_Height;
    }

    int32 Image::getChannelCount() const {
        return m_ImageData->m_ChannelCount;
    }

    const void* Image::getData() const {
        return m_ImageData->getData();
    }

    void Image::extendAlpha(ubyte alpha) {
        if (m_ImageData->m_ChannelCount == 3){
            m_ImageData->extendAlpha(alpha);
            m_ImageData->m_ChannelCount = 4;
        }
    }
}