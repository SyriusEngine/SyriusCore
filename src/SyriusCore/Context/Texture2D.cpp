#include "../../../../include/SyriusCore/Context/Texture2D.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    Texture2D::Texture2D(const Texture2DDesc& desc)
    : m_Width(desc.m_Width),
    m_Height(desc.m_Height),
    m_Format(desc.m_Format){
        SR_CORE_PRECONDITION(m_Width > 0, "Texture width must be greater than 0");
        SR_CORE_PRECONDITION(m_Height > 0, "Texture height must be greater than 0");
        SR_CORE_PRECONDITION(desc.m_Sampler.isValid(), "Sampler must not be null");
    }

    Texture2D::Texture2D(const Texture2DImageDesc &desc)
    : m_Width(desc.m_Image->getWidth()),
    m_Height(desc.m_Image->getHeight()){
        switch (desc.m_Image->getChannelCount()){
            case 1: m_Format = SR_TEXTURE_DATA_FORMAT_R_UI8; break;
            case 2: m_Format = SR_TEXTURE_DATA_FORMAT_RG_UI8; break;
            case 3: m_Format = SR_TEXTURE_DATA_FORMAT_RGB_UI8; break;
            case 4: m_Format = SR_TEXTURE_DATA_FORMAT_RGBA_UI8; break;
            default: {
                SR_CORE_WARNING("Unsupported image format: %i", desc.m_Image->getChannelCount());
                m_Format = SR_TEXTURE_DATA_FORMAT_RGBA_UI8;
            }
        }

    }

    Texture2D::~Texture2D() {

    }

    uint32 Texture2D::getWidth() const {
        return m_Width;
    }

    uint32 Texture2D::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_DATA_FORMAT Texture2D::getFormat() const {
        return m_Format;
    }

}