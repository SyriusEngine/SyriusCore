#include "GlCubemap.hpp"

namespace Syrius{

    GlCubemap::GlCubemap(const CubemapDesc &desc) :
    Cubemap(desc), m_TextureID(0) {
        m_GlDataType = getGlTextureDataType(m_Format);
        m_GlInternalFormat = getGlTextureInternalFormat(m_Format);
        m_GlFormat = getGlTextureType(getTextureFormat(m_Format));

        CubemapFaceDesc faces[] = {
                desc.right,
                desc.left,
                desc.top,
                desc.bottom,
                desc.front,
                desc.back
        };

        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, m_GlInternalFormat, m_Width, m_Height);
        for (uint32 i = 0; i < 6; i++){
            glTextureSubImage3D(m_TextureID, 0, 0, 0, i, m_Width, m_Height, 1, m_GlFormat, m_GlDataType, faces[i].data);
        }

    }

    GlCubemap::GlCubemap(const CubemapImageDesc &desc) :
    Cubemap(desc), m_TextureID(0) {
        m_GlDataType = getGlTextureDataType(m_Format);
        m_GlInternalFormat = getGlTextureInternalFormat(m_Format);
        m_GlFormat = getGlTextureType(getTextureFormat(m_Format));

        CubemapFaceDesc faces[] = {
                {desc.right->getWidth(), desc.right->getHeight(), desc.right->getFormat(), desc.right->getData()},
                {desc.left->getWidth(), desc.left->getHeight(), desc.left->getFormat(), desc.left->getData()},
                {desc.top->getWidth(), desc.top->getHeight(), desc.top->getFormat(), desc.top->getData()},
                {desc.bottom->getWidth(), desc.bottom->getHeight(), desc.bottom->getFormat(), desc.bottom->getData()},
                {desc.front->getWidth(), desc.front->getHeight(), desc.front->getFormat(), desc.front->getData()},
                {desc.back->getWidth(), desc.back->getHeight(), desc.back->getFormat(), desc.back->getData()}
        };

        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, m_GlInternalFormat, m_Width, m_Height);
        for (uint32 i = 0; i < 6; i++){
            glTextureSubImage3D(m_TextureID, 0, 0, 0, i, m_Width, m_Height, 1, m_GlFormat, m_GlDataType, faces[i].data);
        }
    }

    GlCubemap::~GlCubemap() {
        glDeleteTextures(1, &m_TextureID);
    }

    void GlCubemap::bind(uint32 slot) {
        glBindTextureUnit(slot, m_TextureID);
    }

    uint64 GlCubemap::getIdentifier() const {
        return m_TextureID;
    }
}