#include "../../../include/SyriusCore/Image/Image.hpp"

namespace Syrius{

    Image::~Image() = default;

    uint32 Image::getWidth() const {
        return m_Width;
    }

    uint32 Image::getHeight() const {
        return m_Height;
    }

    uint32 Image::getChannelCount() const {
        return getTextureFormatChannelCount(m_Format);
    }

    SR_TEXTURE_FORMAT Image::getFormat() const {
        return m_Format;
    }

    Image::Image():
    m_Width(0),
    m_Height(0),
    m_Format(SR_TEXTURE_NONE){

    }

    Image::Image(uint32 width, uint32 height, SR_TEXTURE_FORMAT format):
    m_Width(width),
    m_Height(height),
    m_Format(format){

    }
}