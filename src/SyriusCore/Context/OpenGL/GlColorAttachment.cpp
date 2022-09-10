#include "GlColorAttachment.hpp"

namespace Syrius{

    GlColorAttachment::GlColorAttachment(const ColorAttachmentDesc &desc)
    : ColorAttachment(desc),
    m_InternalFormat(getGlTextureInternalFormat(desc.m_Format)),
    m_TextureID(0){
        SR_TEXTURE_FORMAT baseFormat = getTextureFormat(desc.m_Format);
        m_GlFormat = getGlTextureType(baseFormat);
        m_ChannelCount = getTextureChannelCount(baseFormat);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, getGlTextureFilter(desc.m_MinFilter));
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, getGlTextureFilter(desc.m_MagFilter));

        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, getGlTextureWrap(desc.m_WrapAddressU));
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, getGlTextureWrap(desc.m_WrapAddressV));

        glTextureStorage2D(m_TextureID, 1, m_InternalFormat, m_Width, m_Height);
    }

    GlColorAttachment::~GlColorAttachment() {
        glDeleteTextures(1, &m_TextureID);
    }

    void GlColorAttachment::bind(uint32 slot) {
        glBindTextureUnit(slot, m_TextureID);
    }

    Image *GlColorAttachment::getData() {
        auto bufSize = m_Width * m_Height * m_ChannelCount;
        auto buf = new uint8_t[bufSize];
        glGetTextureImage(m_TextureID, 0, m_Format, GL_UNSIGNED_BYTE, bufSize, buf);
        auto img = new Image(buf, m_Width, m_Height, m_ChannelCount);
        delete[] buf;
        return img;
    }

    void GlColorAttachment::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        glTextureStorage2D(m_TextureID, 1, m_InternalFormat, m_Width, m_Height);
    }

    uint64 GlColorAttachment::getIdentifier() const {
        return m_TextureID;
    }

}
