#include "../../../include/SyriusCore/Context/Texture2D.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    Texture2D::Texture2D(const Texture2DDesc& desc, const UP<DeviceLimits>& deviceLimits):
    m_DeviceLimits(deviceLimits),
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format),
    m_Usage(desc.usage){
        SR_PRECONDITION(m_Width > 0, "[Texture2D]: Width must be greater than 0");
        SR_PRECONDITION(m_Height > 0, "[Texture2D]: Height must be greater than 0");
        SR_PRECONDITION(m_Width <= m_DeviceLimits->getMaxTextureSize(), "[Texture2D]: Supplied texture width (%i), exceeds device limit (%i)", m_Width, m_DeviceLimits->getMaxTextureSize());
        SR_PRECONDITION(m_Height <= m_DeviceLimits->getMaxTextureSize(), "[Texture2D]: Supplied texture height (%i), exceeds device limit (%i)", m_Height, m_DeviceLimits->getMaxTextureSize());
    }

    Texture2D::Texture2D(const Texture2DImageDesc &desc, const UP<DeviceLimits>& deviceLimits):
    m_DeviceLimits(deviceLimits),
    m_Width(desc.image->getWidth()),
    m_Height(desc.image->getHeight()),
    m_Format(desc.image->getFormat()),
    m_Usage(desc.usage){
        SR_PRECONDITION(m_Width > 0, "[Texture2D]: Width must be greater than 0");
        SR_PRECONDITION(m_Height > 0, "[Texture2D]: Height must be greater than 0");
        SR_PRECONDITION(m_Width <= m_DeviceLimits->getMaxTextureSize(), "[Texture2D]: Supplied texture width (%i), exceeds device limit (%i)", m_Width, m_DeviceLimits->getMaxTextureSize());
        SR_PRECONDITION(m_Height <= m_DeviceLimits->getMaxTextureSize(), "[Texture2D]: Supplied texture height (%i), exceeds device limit (%i)", m_Height, m_DeviceLimits->getMaxTextureSize());
    }

    Texture2D::~Texture2D() {

    }

    u32 Texture2D::getWidth() const {
        return m_Width;
    }

    u32 Texture2D::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_FORMAT Texture2D::getFormat() const {
        return m_Format;
    }

    SR_BUFFER_USAGE Texture2D::getUsage() const {
        return m_Usage;
    }
}