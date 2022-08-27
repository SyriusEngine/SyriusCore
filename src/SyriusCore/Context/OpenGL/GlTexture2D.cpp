#include "GlTexture2D.hpp"

namespace Syrius{

    GlTexture2D::GlTexture2D()
    : Texture2D(),
    m_TextureID(0){
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

    }

    GlTexture2D::~GlTexture2D() {
        glDeleteTextures(1, &m_TextureID);
    }

    void GlTexture2D::bind(uint32_t slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

    void GlTexture2D::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GlTexture2D::upload(Image *image) {
        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        GLenum format;
        switch (image->getChannelCount()){
            case 4:
                format = GL_RGBA8;
                break;
            case 3:
                format = GL_RGB8;
                break;
            case 2:
                format = GL_RG8;
                break;
            case 1:
                format = GL_R8;
                break;
            default:
                SR_CORE_WARNING("Invalid texture format, default format RGBA will be picked");
                format = GL_RGBA8;
                break;
        }

        glTextureStorage2D(m_TextureID, 1, format, image->getWidth(), image->getHeight());
        glTextureSubImage2D(m_TextureID, 0, 0, 0, image->getWidth(), image->getHeight(), format, GL_UNSIGNED_BYTE, &image->getPixels()[0]);
        glGenerateTextureMipmap(m_TextureID);
    }

    uint64 GlTexture2D::getIdentifier() const {
        return m_TextureID;
    }
}