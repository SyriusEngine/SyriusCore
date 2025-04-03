#include "GlVertexBuffer.hpp"

namespace Syrius{

    GlVertexBuffer::GlVertexBuffer(const VertexBufferDesc &desc, const UP<DeviceLimits>& deviceLimits):
    VertexBuffer(desc, deviceLimits),
    m_BufferID(0){
        glCreateBuffers(1, &m_BufferID);
        glNamedBufferData(m_BufferID, m_Size, desc.data, getGlBufferType(m_Usage));
    }

    GlVertexBuffer::~GlVertexBuffer() {
        glDeleteBuffers(1, &m_BufferID);
    }

    void GlVertexBuffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
    }

    void GlVertexBuffer::setData(const void *data, u32 count) {
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlVertexBuffer]: Update on buffer object {} requested, which was created with SR_BUFFER_USAGE_STATIC flag!", m_BufferID);
        SR_PRECONDITION(count * m_Layout->getStride() <= m_Size, "[GlVertexBuffer]: Update on buffer object {} requested, which exceeds the current buffer size (%i > %i).", m_BufferID, count * m_Layout->getStride(), m_Size);

        u32 copySize = count * m_Layout->getStride();
        m_Count = count;

        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_WRITE_ONLY);
        if (!pBuffer){
            SR_LOG_WARNING("GlVertexBuffer", "Failed to map buffer object {}", m_BufferID);
            return;
        }

        memcpy(pBuffer, data, copySize);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        if (retVal == GL_FALSE){
            SR_LOG_WARNING("GlVertexBuffer", "Failed to unmap buffer object {}", m_BufferID);
        }
    }

    void GlVertexBuffer::copyFrom(const ResourceView<VertexBuffer> &other) {
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlVertexBuffer]: Update on buffer object {} requested, which was created with SR_BUFFER_USAGE_STATIC flag!", m_BufferID);

        if (m_Size < other->getSize()){
            SR_LOG_WARNING("GlVertexBuffer", "Copy from buffer object {} requested, which exceeds the current buffer size ({} > {}).", m_BufferID, other->getSize(), m_Size);
        }
        auto copySize = std::min(m_Size, other->getSize());
        glCopyNamedBufferSubData(other->getIdentifier(), m_BufferID, 0, 0, copySize);
    }

    UP<UByte[]> GlVertexBuffer::getData() const {
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlVertexBuffer]: Update on buffer object {} requested, which was created with SR_BUFFER_USAGE_STATIC flag!", m_BufferID);

        auto data = createUP<UByte[]>(m_Size);
        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_READ_ONLY);

        if (!pBuffer){
            SR_LOG_WARNING("GlVertexBuffer", "Failed to map buffer object {}", m_BufferID);
            return std::move(data);
        }

        memcpy(data.get(), pBuffer, m_Size);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        if (retVal == GL_FALSE){
            SR_LOG_WARNING("GlVertexBuffer", "Failed to unmap buffer object {}", m_BufferID);
        }
        return std::move(data);
    }

    u64 GlVertexBuffer::getIdentifier() const {
        return m_BufferID;
    }

}
