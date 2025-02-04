#include "../../../include/SyriusCore/Context/CubeMapLayout.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    CubeMapLayout::CubeMapLayout(u32 width, u32 height, SR_TEXTURE_FORMAT format):
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

    void CubeMapLayout::addFace(SR_CUBEMAP_FACE faceID, UP<Image>& image) {
        if (image->getFormat() != m_Format){
            SR_LOG_WARNING("CubeMapLayout", "Image format (%i) does not match the specified cubemap format (%i), "
                            "the face will not be loaded!", image->getFormat(), m_Format);
            return;
        }
        if (image->getWidth() != m_Width or image->getHeight() != m_Height){
            SR_LOG_WARNING("CubeMapLayout", "Image dimensions (%ix%i) do not match the specified cubemap dimensions (%ix%i)."
                            "The image will be resized to match the cubemap dimensions! Consider resizing the image before loading it"
                            "to avoid performance issues!", image->getWidth(), image->getHeight(), m_Width, m_Height);
            image->resize(m_Width, m_Height);
        }
        m_Faces[faceID] = std::move(image);
    }

    void CubeMapLayout::addFaceFromImage(SR_CUBEMAP_FACE faceID, const ImageFileDesc &desc) {
        auto image = createImage(desc);
        if (image->getFormat() == SR_TEXTURE_RGB_UI8){
            SR_LOG_WARNING("CubeMapLayout", "Image format is RGB, extending to RGBA to avoid issues on some GPUs!");
            image->extendAlpha(); // GPUs dont like 3 channel images (D3D11 in particular)
        }
        addFace(faceID, image);
    }

    void CubeMapLayout::addFaceFromFile(SR_CUBEMAP_FACE faceID, const fs::path &file) {
        checkFile(file);

        ImageFileDesc desc;
        desc.flipOnAccess = false;
        desc.fileName = file;
        addFaceFromImage(faceID, desc);
    }

    u32 CubeMapLayout::getWidth() const {
        return m_Width;
    }

    u32 CubeMapLayout::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_FORMAT CubeMapLayout::getFormat() const {
        return m_Format;
    }

    const UP<Image> &CubeMapLayout::getFace(SR_CUBEMAP_FACE faceID) const {
        return m_Faces[faceID];
    }

    const std::array<UP<Image>, 6>& CubeMapLayout::getFaces() const {
        return m_Faces;
    }
}
