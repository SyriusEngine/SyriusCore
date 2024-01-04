#include "../../../include/SyriusCore/Context/Cubemap.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    Cubemap::Cubemap(const CubemapDesc &desc):
    m_Width(desc.right.width),
    m_Height(desc.right.height),
    m_Format(desc.right.format){
        CubemapFaceDesc faces[] = {
                desc.right,
                desc.left,
                desc.top,
                desc.bottom,
                desc.front,
                desc.back
        };
        for (uint32 i = 0; i < 6; ++i) {
            SR_CORE_ASSERT(m_Width == faces[i].width, "Cubemap %p is configured to have width %u, but face %u has width %u", this, m_Width, i, faces[i].width);
            SR_CORE_ASSERT(m_Height == faces[i].height, "Cubemap %p is configured to have height %u, but face %u has height %u", this, m_Height, i, faces[i].height);
            SR_CORE_ASSERT(m_Format == faces[i].format, "Cubemap %p is configured to have format %u, but face %u has format %u", this, m_Format, i, faces[i].format);
        }
    }

    Cubemap::Cubemap(const CubemapImageDesc &desc):
    m_Width(desc.right->getWidth()),
    m_Height(desc.right->getHeight()),
    m_Format(desc.right->getFormat()){
        CubemapFaceDesc faces[] = {
                {desc.right->getWidth(), desc.right->getHeight(), desc.right->getFormat(), desc.right->getData()},
                {desc.left->getWidth(), desc.left->getHeight(), desc.left->getFormat(), desc.left->getData()},
                {desc.top->getWidth(), desc.top->getHeight(), desc.top->getFormat(), desc.top->getData()},
                {desc.bottom->getWidth(), desc.bottom->getHeight(), desc.bottom->getFormat(), desc.bottom->getData()},
                {desc.front->getWidth(), desc.front->getHeight(), desc.front->getFormat(), desc.front->getData()},
                {desc.back->getWidth(), desc.back->getHeight(), desc.back->getFormat(), desc.back->getData()}
        };

        for (uint32 i = 0; i < 6; ++i) {
            SR_CORE_ASSERT(m_Width == faces[i].width, "Cubemap %p is configured to have width %u, but face %u has width %u", this, m_Width, i, faces[i].width);
            SR_CORE_ASSERT(m_Height == faces[i].height, "Cubemap %p is configured to have height %u, but face %u has height %u", this, m_Height, i, faces[i].height);
            SR_CORE_ASSERT(m_Format == faces[i].format, "Cubemap %p is configured to have format %u, but face %u has format %u", this, m_Format, i, faces[i].format);
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
