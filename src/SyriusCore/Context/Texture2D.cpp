#include "../../../include/SyriusCore/Context/Texture2D.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    Texture2D::Texture2D(const Texture2DDesc& desc):
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format){
        SR_CORE_PRECONDITION(m_Width > 0, "Texture width must be greater than 0");
        SR_CORE_PRECONDITION(m_Height > 0, "Texture height must be greater than 0");
    }

    Texture2D::Texture2D(const Texture2DImageDesc &desc):
    m_Width(desc.image->getWidth()),
    m_Height(desc.image->getHeight()),
    m_Format(desc.image->getFormat()){
        SR_CORE_PRECONDITION(m_Width > 0, "Texture width must be greater than 0");
        SR_CORE_PRECONDITION(m_Height > 0, "Texture height must be greater than 0");

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