#include "GlTexture2D.hpp"

namespace Syrius{

    GlTexture2D::GlTexture2D(const Texture2DDesc& desc)
    : Texture2D(desc),
    m_TextureID(0){
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        m_GlDataType = getGlTextureDataType(m_Format);

        setGlFormats();

        glTextureStorage2D(m_TextureID, 1, m_GlInternalFormat, m_Width, m_Height);
        // it is possible to create an empty texture. If so, skip following call
        if (desc.data != nullptr){
            glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, m_GlFormat, m_GlDataType, desc.data);
        }
        glGenerateTextureMipmap(m_TextureID);

    }

    GlTexture2D::GlTexture2D(const Texture2DImageDesc &desc)
    : Texture2D(desc),
    m_TextureID(0){
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        m_GlDataType = getGlTextureDataType(m_Format);

        setGlFormats();

        glTextureStorage2D(m_TextureID, 1, m_GlInternalFormat, m_Width, m_Height);
        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, m_GlFormat, m_GlDataType, desc.image->getData());
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

    void GlTexture2D::setData(const void *data, uint32 x, uint32 y, uint32 width, uint32 height) {
        SR_CORE_PRECONDITION(data != nullptr, "Data must not be null");
        SR_CORE_PRECONDITION(x < m_Width, "x is out of range");
        SR_CORE_PRECONDITION(y < m_Height, "y is out of range");
        SR_CORE_PRECONDITION(x + width <= m_Width, "x + width is out of range");
        SR_CORE_PRECONDITION(y + height <= m_Height, "y + height is out of range");

        glTextureSubImage2D(m_TextureID, 0, x, y, width, height, m_GlFormat, m_GlDataType, data);

    }

    Resource<Image> GlTexture2D::getData() {
        auto channelCount = getTextureDataChannelCount(m_Format);
        auto size = m_Width * m_Height * channelCount;
        auto data = Resource<uint8>(new uint8[size]);
        glGetTextureImage(m_TextureID, 0, m_GlFormat, m_GlDataType, size, data.get());
        return createResource<Image>(data.get(), m_Width, m_Height, m_Format);

    }

    uint64 GlTexture2D::getIdentifier() const {
        return m_TextureID;
    }

    void GlTexture2D::setGlFormats() {
        auto channelCount = getTextureDataChannelCount(m_Format);
        switch (channelCount){
            case 4:
                m_GlFormat = GL_RGBA;
                m_GlInternalFormat = GL_RGBA8;
                break;
            case 3:
                m_GlFormat = GL_RGB;
                m_GlInternalFormat = GL_RGB8;
                break;
            case 2:
                m_GlFormat = GL_RG;
                m_GlInternalFormat = GL_RG8;
                break;
            case 1:
                m_GlFormat = GL_RED;
                m_GlInternalFormat = GL_R8;
                break;
            default: {
                SR_CORE_WARNING("Invalid texture format, default format RGBA will be picked");
                m_GlFormat = GL_RGBA;
                m_GlInternalFormat = GL_RGBA8;
                break;
            }
        }
    }

}