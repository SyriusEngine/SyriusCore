#include "GlVertexBuffer.hpp"

namespace Syrius{

    GlVertexBuffer::GlVertexBuffer(const VertexBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits):
    VertexBuffer(desc, deviceLimits),
    m_BufferID(0){
        createBuffer(desc.data);
    }

    GlVertexBuffer::~GlVertexBuffer() {
        glDeleteBuffers(1, &m_BufferID);
    }

    void GlVertexBuffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
    }

    void GlVertexBuffer::setData(const void *data, uint32 count) {
        SR_CORE_PRECONDITION(m_Usage == SR_BUFFER_USAGE_DYNAMIC, "[VertexBuffer]: Update on buffer object (%p) requested, which has not been created with SR_BUFFER_USAGE_DYNAMIC flag!", this);
        SR_CORE_PRECONDITION(count * m_Layout->getStride() <= m_Size, "[VertexBuffer]: Update on buffer object (%p) requested, which exceeds the current buffer size (%i > %i).", this, count * m_Layout->getStride(), m_Size);

        uint32 newSize = count * m_Layout->getStride();
        m_Count = count;

        if (newSize <= m_Size) {
            glNamedBufferSubData(m_BufferID, 0, m_Size, data);
        }
    }

    uint64 GlVertexBuffer::getIdentifier() const {
        return m_BufferID;
    }

    void GlVertexBuffer::createBuffer(const void *data) {
        glCreateBuffers(1, &m_BufferID);
        glNamedBufferData(m_BufferID, m_Size, data, getGlBufferUsage(m_Usage));
    }


}
