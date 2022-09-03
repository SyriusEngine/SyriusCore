#include "GlVertexBuffer.hpp"

namespace Syrius{

    GlVertexBuffer::GlVertexBuffer(const VertexBufferDesc &desc)
    : VertexBuffer(desc),
    m_BufferID(0){
        glCreateBuffers(1, &m_BufferID);
        glNamedBufferData(m_BufferID, m_Size, desc.m_Data, getGlBufferType(desc.m_Type));
    }

    GlVertexBuffer::~GlVertexBuffer() {
        glDeleteBuffers(1, &m_BufferID);
    }

    void GlVertexBuffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
    }

    void GlVertexBuffer::setData(const void *data) {
        glNamedBufferSubData(m_BufferID, 0, m_Size, data);
    }

    uint64 GlVertexBuffer::getIdentifier() const {
        return m_BufferID;
    }

}
