#include "../../../include/SyriusCore/Context/CubeMapLayout.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    CubeMapLayout::CubeMapLayout(uint32 width, uint32 height, SR_TEXTURE_FORMAT format, const Resource<DeviceLimits> &deviceLimits):
    m_DeviceLimits(deviceLimits),
    m_Width(width),
    m_Height(height),
    m_Format(format),
    m_Faces() {
        m_Faces[0] = nullptr;
        m_Faces[1] = nullptr;
        m_Faces[2] = nullptr;
        m_Faces[3] = nullptr;
        m_Faces[4] = nullptr;
        m_Faces[5] = nullptr;
    }

    CubeMapLayout::~CubeMapLayout() = default;

    void CubeMapLayout::addFace(SR_CUBEMAP_FACE faceID, Resource<Image>& image) {
        if (image->getFormat() != m_Format){
            SR_CORE_WARNING("[CubeMapLayout]: Image format (%i) does not match the specified cubemap format (%i), "
                            "the face will not be loaded!", image->getFormat(), m_Format);
            return;
        }
        if (image->getWidth() != m_Width or image->getHeight() != m_Height){
            SR_CORE_WARNING("[CubeMapLayout]: Image dimensions (%ix%i) do not match the specified cubemap dimensions (%ix%i)."
                            "The image will be resized to match the cubemap dimensions! Consider resizing the image before loading it"
                            "to avoid performance issues!", image->getWidth(), image->getHeight(), m_Width, m_Height);
            image->resize(m_Width, m_Height);
        }
        m_Faces[faceID] = std::move(image);
    }

    void CubeMapLayout::addFaceFromFile(SR_CUBEMAP_FACE faceID, const ImageFileDesc &desc) {
        auto image = createImage(desc);
        addFace(faceID, image);
    }

    uint32 CubeMapLayout::getWidth() const {
        return m_Width;
    }

    uint32 CubeMapLayout::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_FORMAT CubeMapLayout::getFormat() const {
        return m_Format;
    }

    const Resource<Image> &CubeMapLayout::getFace(SR_CUBEMAP_FACE faceID) const {
        return m_Faces[faceID];
    }

    const std::array<Resource<Image>, 6>& CubeMapLayout::getFaces() const {
        return m_Faces;
    }
}
