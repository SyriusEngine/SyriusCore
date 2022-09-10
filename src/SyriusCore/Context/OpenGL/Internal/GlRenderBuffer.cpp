#include "GlRenderBuffer.hpp"
#include "../GlColorAttachment.hpp"


namespace Syrius{

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

