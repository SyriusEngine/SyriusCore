#include "../../../include/SyriusCore/Context/Texture2D.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    Texture2D::Texture2D(const Texture2DDesc& desc, const Resource<DeviceLimits>& deviceLimits):
    m_DeviceLimits(deviceLimits),
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format){
    }

    Texture2D::Texture2D(const Texture2DImageDesc &desc, const Resource<DeviceLimits>& deviceLimits):
    m_DeviceLimits(deviceLimits),
    m_Width(desc.image->getWidth()),
    m_Height(desc.image->getHeight()),
    m_Format(desc.image->getFormat()){

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