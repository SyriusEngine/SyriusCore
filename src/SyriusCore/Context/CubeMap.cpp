#include "../../../include/SyriusCore/Context/CubeMap.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    CubeMap::CubeMap(const CubemapDesc &desc, const Resource<DeviceLimits>& deviceLimits):
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format),
    m_DeviceLimits(deviceLimits){
        
    }

    CubeMap::CubeMap(const CubemapImageDesc &desc, const Resource<DeviceLimits>& deviceLimits):
    m_Width(desc.faces[0]->getWidth()),
    m_Height(desc.faces[0]->getHeight()),
    m_Format(desc.faces[0]->getFormat()),
    m_DeviceLimits(deviceLimits){
        
        for (uint32 i = 0; i < 6; ++i) {
            SR_CORE_ASSERT(m_Width == desc.faces[i]->getWidth(), "Cubemap %p is configured to have width %u, but face %u has width %u", this, m_Width, i, desc.faces[i]->getWidth());
            SR_CORE_ASSERT(m_Height == desc.faces[i]->getHeight(), "Cubemap %p is configured to have height %u, but face %u has height %u", this, m_Height, i, desc.faces[i]->getHeight());
            SR_CORE_ASSERT(m_Format == desc.faces[i]->getFormat(), "Cubemap %p is configured to have format %u, but face %u has format %u", this, m_Format, i, desc.faces[i]->getFormat());
        }
    }

    CubeMap::~CubeMap() = default;

    uint32 CubeMap::getWidth() const {
        return m_Width;
    }

    uint32 CubeMap::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_FORMAT CubeMap::getFormat() const {
        return m_Format;
    }
}
