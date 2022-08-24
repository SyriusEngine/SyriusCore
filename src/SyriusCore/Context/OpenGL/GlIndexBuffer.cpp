#include "GlIndexBuffer.hpp"

namespace Syrius{

    GlIndexBuffer::GlIndexBuffer(const IndexBufferDesc &desc)
    : IndexBuffer(desc),
    m_BufferID(0){
        glGenBuffers(1, &m_BufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Size, desc.m_Data, m_Type);
    }

    GlIndexBuffer::~GlIndexBuffer() {
        glDeleteBuffers(1, &m_BufferID);
    }

    void GlIndexBuffer::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
    }

    void GlIndexBuffer::unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}