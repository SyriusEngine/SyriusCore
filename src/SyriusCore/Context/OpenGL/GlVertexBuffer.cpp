#include "GlVertexBuffer.hpp"

namespace Syrius{

    GlVertexBuffer::GlVertexBuffer(const VertexBufferDesc &desc)
    : VertexBuffer(desc),
    m_BufferID(0){
        glGenBuffers(1, &m_BufferID);
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
        glBufferData(GL_ARRAY_BUFFER, m_Size, desc.m_Data, m_Type);
    }

    GlVertexBuffer::~GlVertexBuffer() {
        glDeleteBuffers(1, &m_BufferID);
    }

    void GlVertexBuffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
    }

    void GlVertexBuffer::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

}
