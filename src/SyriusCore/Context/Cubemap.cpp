#include "../../../include/SyriusCore/Context/Cubemap.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    Cubemap::Cubemap(const CubemapDesc &desc):
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format){
        
    }

    Cubemap::Cubemap(const CubemapImageDesc &desc):
    m_Width(desc.faces[0]->getWidth()),
    m_Height(desc.faces[0]->getHeight()),
    m_Format(desc.faces[0]->getFormat()){
        
        for (uint32 i = 0; i < 6; ++i) {
            SR_CORE_ASSERT(m_Width == desc.faces[i]->getWidth(), "Cubemap %p is configured to have width %u, but face %u has width %u", this, m_Width, i, desc.faces[i]->getWidth());
            SR_CORE_ASSERT(m_Height == desc.faces[i]->getHeight(), "Cubemap %p is configured to have height %u, but face %u has height %u", this, m_Height, i, desc.faces[i]->getHeight());
            SR_CORE_ASSERT(m_Format == desc.faces[i]->getFormat(), "Cubemap %p is configured to have format %u, but face %u has format %u", this, m_Format, i, desc.faces[i]->getFormat());
        }
    }

    Cubemap::~Cubemap() {

    }

    uint32 Cubemap::getWidth() const {
        return m_Width;
    }

    uint32 Cubemap::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_FORMAT Cubemap::getFormat() const {
        return m_Format;
    }
}
