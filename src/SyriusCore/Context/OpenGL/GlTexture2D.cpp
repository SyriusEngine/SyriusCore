#include "GlTexture2D.hpp"

namespace Syrius{

    GlTexture2D::GlTexture2D(const Texture2DDesc& desc)
    : Texture2D(desc),
    m_TextureID(0){
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

        GLenum format;
        GLint internalFormat;
        uint32 width = desc.m_Image->getWidth();
        uint32 height = desc.m_Image->getHeight();
        switch (desc.m_Image->getChannelCount()){
            case 4:
                format = GL_RGBA;
                internalFormat = GL_RGBA8;
                break;
            case 3:
                format = GL_RGB;
                internalFormat = GL_RGB8;
                break;
            case 2:
                format = GL_RG;
                internalFormat = GL_RG8;
                break;
            case 1:
                format = GL_RED;
                internalFormat = GL_R8;
                break;
            default: {
                SR_CORE_WARNING("Invalid texture format, default format RGBA will be picked");
                format = GL_RGBA;
                internalFormat = GL_RGBA8;
                break;
            }
        }

        glTextureStorage2D(m_TextureID, 1, internalFormat, width, height);
        glTextureSubImage2D(m_TextureID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, &desc.m_Image->getData()[0]);
        glGenerateTextureMipmap(m_TextureID);

    }

    GlTexture2D::~GlTexture2D() {
        glDeleteTextures(1, &m_TextureID);
    }

    void GlTexture2D::bind(uint32_t slot) {
        glBindTextureUnit(slot, m_TextureID);
    }

    void GlTexture2D::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    uint64 GlTexture2D::getIdentifier() const {
        return m_TextureID;
    }
}