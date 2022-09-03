#include "GlIndexBuffer.hpp"

namespace Syrius{

    GlIndexBuffer::GlIndexBuffer(const IndexBufferDesc &desc)
    : IndexBuffer(desc),
    m_BufferID(0){
        glCreateBuffers(1, &m_BufferID);
        glNamedBufferData(m_BufferID, m_Size, desc.m_Data, getGlBufferType(desc.m_Type));
    }

    GlIndexBuffer::~GlIndexBuffer() {
        glDeleteBuffers(1, &m_BufferID);
    }

    void GlIndexBuffer::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
    }

    void GlIndexBuffer::setData(const void *data) {
        glNamedBufferSubData(m_BufferID, 0, m_Size, data);
    }

    uint64 GlIndexBuffer::getIdentifier() const {
        return m_BufferID;
    }

}