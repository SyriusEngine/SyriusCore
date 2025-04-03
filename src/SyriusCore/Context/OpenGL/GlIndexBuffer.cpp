#include "GlIndexBuffer.hpp"

namespace Syrius{

    GlIndexBuffer::GlIndexBuffer(const IndexBufferDesc &desc, const UP<DeviceLimits>& deviceLimits):
    IndexBuffer(desc, deviceLimits),
    m_BufferID(0){
        glCreateBuffers(1, &m_BufferID);
        glNamedBufferData(m_BufferID, m_Size, desc.data, getGlBufferType(desc.usage));
    }

    GlIndexBuffer::~GlIndexBuffer() {
        glDeleteBuffers(1, &m_BufferID);
    }

    void GlIndexBuffer::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
    }

    void GlIndexBuffer::setData(const void *data, u32 count) {
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlIndexBuffer]: Update on buffer object {} requested, which was created with SR_BUFFER_USAGE_STATIC flag!", m_BufferID);
        SR_PRECONDITION(count * getTypeSize(m_DataType) <= m_Size, "[GlIndexBuffer]: Update on buffer object {} requested, which exceeds the current buffer size ({} > {}).", m_BufferID, count * getTypeSize(m_DataType), m_Size);


        u32 copySize = count * getTypeSize(m_DataType);
        m_Count = count;
        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_WRITE_ONLY);
        if (!pBuffer) {
            SR_LOG_WARNING("GlIndexBuffer", "Failed to map buffer object {}", m_BufferID);
            return;
        }

        memcpy(pBuffer, data, copySize);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        if (retVal == GL_FALSE) {
            SR_LOG_WARNING("GlIndexBuffer", "Failed to unmap buffer object {}", m_BufferID);
        }
    }

    void GlIndexBuffer::copyFrom(const ResourceView<IndexBuffer> &other) {
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlIndexBuffer]: Update on buffer object {} requested, which was created with SR_BUFFER_USAGE_STATIC flag!", m_BufferID);

        if (m_Size < other->getSize()){
            SR_LOG_WARNING("GlIndexBuffer", "Copy from buffer object {} requested, which exceeds the current buffer "
                                            "size ({} > {}).", m_BufferID, other->getSize(), m_Size);
        }
        auto copySize = std::min(m_Size, other->getSize());
        glCopyNamedBufferSubData(other->getIdentifier(), m_BufferID, 0, 0, copySize);
    }

    UP<UByte[]> GlIndexBuffer::getData() const {
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlIndexBuffer]: Update on buffer object [] requested, which was created with SR_BUFFER_USAGE_STATIC flag!", m_BufferID);

        auto data = createUP<UByte[]>(m_Size);
        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_READ_ONLY);

        if (!pBuffer) {
            SR_LOG_WARNING("GlIndexBuffer", "Failed to map buffer object {}", m_BufferID);
            return std::move(data);
        }

        memcpy(data.get(), pBuffer, m_Size);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        if (retVal == GL_FALSE) {
            SR_LOG_WARNING("GlIndexBuffer", "Failed to unmap buffer object {}", m_BufferID);
        }
        return std::move(data);
    }

    u64 GlIndexBuffer::getIdentifier() const {
        return m_BufferID;
    }

}