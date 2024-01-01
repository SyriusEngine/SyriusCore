#include "ImageImpl.hpp"

namespace Syrius{

    ImageImpl::ImageImpl(const ImageDesc &desc):
    Image(desc.width, desc.height, desc.format){
        auto channels = getTextureFormatChannelCount(desc.format);
        auto dataType = getTextureDataType(desc.format);
        auto typeSize = getTypeSize(dataType);

        // size in bytes
        m_Size = desc.width * desc.height * channels * typeSize;

        // allocate memory
        m_Data = malloc(m_Size);
        memcpy(m_Data, desc.data, m_Size);
    }

    ImageImpl::~ImageImpl() {
        free(m_Data);
    }

    void ImageImpl::writeToFile(const ImageFileDesc &writeDesc) const {
        printf("Cannot write image with format %d to file\n", m_Format);
    }

    void ImageImpl::resize(uint32 width, uint32 height) {
        printf("Not Implemented\n");
    }

    void ImageImpl::extendAlpha() {
        printf("Not Implemented\n");
    }

    const void *ImageImpl::getData() const {
        return m_Data;
    }
}
