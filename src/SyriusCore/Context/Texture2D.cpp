#include "../../../include/SyriusCore/Context/Texture2D.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    Texture2DImageDesc::Texture2DImageDesc(const Resource<Image> &image):
    image(image){

    }

    Texture2D::Texture2D(const Texture2DDesc& desc):
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format){
        SR_CORE_PRECONDITION(m_Width > 0, "Texture width must be greater than 0");
        SR_CORE_PRECONDITION(m_Height > 0, "Texture height must be greater than 0");
    }

    Texture2D::Texture2D(const Texture2DImageDesc &desc):
    m_Width(desc.image->getWidth()),
    m_Height(desc.image->getHeight()){
        SR_CORE_PRECONDITION(m_Width > 0, "Texture width must be greater than 0");
        SR_CORE_PRECONDITION(m_Height > 0, "Texture height must be greater than 0");
        switch (desc.image->getChannelCount()){
            case 1: m_Format = SR_TEXTURE_R_UI8;    break;
            case 2: m_Format = SR_TEXTURE_RG_UI8;   break;
            case 3: m_Format = SR_TEXTURE_RGB_UI8;  break;
            case 4: m_Format = SR_TEXTURE_RGBA_UI8; break;
            default: {
                SR_CORE_WARNING("Unsupported image format: %i", desc.image->getChannelCount());
                m_Format = SR_TEXTURE_RGBA_UI8;
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

    SR_TEXTURE_FORMAT Texture2D::getFormat() const {
        return m_Format;
    }
}