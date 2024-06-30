#include "ImageT.hpp"
#include "ImageUI8.hpp"

namespace Syrius{


    ImageT::ImageT(const ImageDesc &desc):
    Image(desc.width, desc.height, desc.format){
        auto size = desc.width * desc.height * getBytesPerPixel(desc.format);
        m_Data = Resource<UByte>(new UByte[size]);
        if (desc.data != nullptr){
            memcpy(m_Data.get(), desc.data, desc.width * desc.height * getBytesPerPixel(desc.format));
        }
    }

    ImageT::~ImageT() = default;

    void ImageT::writeToFile(const ImageFileDesc &writeDesc) const {
        throw std::runtime_error("[ImageT]: Writing to file not implemented");
    }

    void ImageT::resize(uint32 width, uint32 height) {
        throw std::runtime_error("[ImageT]: Resizing not implemented");
    }

    void ImageT::extendAlpha() {
        throw std::runtime_error("[ImageT]: Alpha extension not implemented");
    }

    void *ImageT::getData() {
        return m_Data.get();
    }

    Resource<Image> ImageT::convertToUI8() {
        ImageUI8Desc desc;
        desc.width = m_Width;
        desc.height = m_Height;
        desc.format = getFormatFromChannelCount(getTextureChannelCount(m_Format), SR_UINT8);
        desc.data = m_Data.get();
        auto img = createResource<ImageUI8>(desc);

        return img;
    }
}
