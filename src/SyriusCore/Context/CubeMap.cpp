#include "../../../include/SyriusCore/Context/CubeMap.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    CubeMap::CubeMap(const ResourceView<CubeMapLayout>& desc, const UP<DeviceLimits>& deviceLimits):
    m_Width(desc->getWidth()),
    m_Height(desc->getHeight()),
    m_Format(desc->getFormat()),
    m_DeviceLimits(deviceLimits){
        
    }

    CubeMap::~CubeMap() = default;

    u32 CubeMap::getWidth() const {
        return m_Width;
    }

    u32 CubeMap::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_FORMAT CubeMap::getFormat() const {
        return m_Format;
    }
}
