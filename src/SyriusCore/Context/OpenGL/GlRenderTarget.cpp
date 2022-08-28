#include "GlRenderTarget.hpp"

namespace Syrius{

    GlColorAttachment::GlColorAttachment(uint32 width, uint32 height, SR_TEXTURE_DATA_FORMAT format)
    : m_Width(width),
    m_Height(height),
    m_InternalFormat(getGlTextureInternalFormat(format)),
    m_TextureID(0) {
        SR_TEXTURE_FORMAT baseFormat = getTextureFormat(format);
        m_Format = getGlTextureType(baseFormat);
        m_ChannelCount = getTextureChannelCount(baseFormat);
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureStorage2D(m_TextureID, 1, m_InternalFormat, m_Width, m_Height);
    }

    GlColorAttachment::~GlColorAttachment() {
        glDeleteTextures(1, &m_TextureID);
    }

    void GlColorAttachment::bind(uint32 slot) const {
        glBindTextureUnit(slot, m_TextureID);
    }

    Image *GlColorAttachment::readColorAttachment() const {
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

    uint32 GlColorAttachment::getIdentifier() const {
        return m_TextureID;
    }


    GlRenderBuffer::GlRenderBuffer(uint32 width, uint32 height)
    : m_Width(width),
    m_Height(height),
    m_BufferID(0),
    m_InternalFormat(GL_DEPTH24_STENCIL8) {
        glCreateRenderbuffers(1, &m_BufferID);

        glNamedRenderbufferStorage(m_BufferID, m_InternalFormat, m_Width, m_Height);
    }

    GlRenderBuffer::~GlRenderBuffer() {
        glDeleteRenderbuffers(1, &m_BufferID);
    }

    void GlRenderBuffer::bind() const {
        glBindRenderbuffer(GL_RENDERBUFFER, m_BufferID);
    }

    void GlRenderBuffer::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        glNamedRenderbufferStorage(m_BufferID, m_InternalFormat, m_Width, m_Height);
    }

    uint32 GlRenderBuffer::getIdentifier() const {
        return m_BufferID;
    }
}

