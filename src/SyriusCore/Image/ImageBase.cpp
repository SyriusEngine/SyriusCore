#include "../../../include/SyriusCore/Image/ImageBase.hpp"

namespace Syrius{

    Image::~Image() = default;

    u32 Image::getWidth() const {
        return m_Width;
    }

    u32 Image::getHeight() const {
        return m_Height;
    }

    u32 Image::getChannelCount() const {
        return getTextureChannelCount(m_Format);
    }

    SR_TEXTURE_FORMAT Image::getFormat() const {
        return m_Format;
    }

    Image::Image():
    m_Width(0),
    m_Height(0),
    m_Format(SR_TEXTURE_NONE){

    }

    Image::Image(u32 width, u32 height, SR_TEXTURE_FORMAT format):
    m_Width(width),
    m_Height(height),
    m_Format(format){

    }
}