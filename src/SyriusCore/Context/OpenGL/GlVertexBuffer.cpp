#include "GlVertexBuffer.hpp"

namespace Syrius{

    GlVertexBuffer::GlVertexBuffer(const VertexBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits):
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

    void GlVertexBuffer::setData(const void *data, uint32 count) {
        SR_CORE_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlVertexBuffer]: Update on buffer object (%p) requested, which was created with SR_BUFFER_USAGE_STATIC flag!", this);
        SR_CORE_PRECONDITION(count * m_Layout->getStride() <= m_Size, "[GlVertexBuffer]: Update on buffer object (%p) requested, which exceeds the current buffer size (%i > %i).", this, count * m_Layout->getStride(), m_Size);

        uint32 copySize = count * m_Layout->getStride();
        m_Count = count;

        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_WRITE_ONLY);
        memcpy(pBuffer, data, copySize);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        SR_CORE_ASSERT(retVal, "[GlVertexBuffer]: Failed to unmap buffer object (%i)", m_BufferID);
    }

    void GlVertexBuffer::copyFrom(const ResourceView<VertexBuffer> &other) {
        SR_CORE_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlVertexBuffer]: Update on buffer object (%p) requested, which was created with SR_BUFFER_USAGE_STATIC flag!", this);

        if (m_Size >= other->getSize()){
            auto copySize = std::min(m_Size, other->getSize());
            glCopyNamedBufferSubData(other->getIdentifier(), m_BufferID, 0, 0, copySize);
        }
        else{
            SR_CORE_WARNING("[GlVertexBuffer]: Copy from buffer object (%p) requested, which exceeds the current buffer "
                            "size (%i > %i).", this, other->getSize(), m_Size);
        }
    }

    Resource<UByte[]> GlVertexBuffer::getData() const {
        SR_CORE_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlVertexBuffer]: Update on buffer object (%p) requested, which was created with SR_BUFFER_USAGE_STATIC flag!", this);

        auto data = createResource<UByte[]>(m_Size);
        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_READ_ONLY);
        if (!pBuffer){
            SR_CORE_THROW("[GlVertexBuffer]: Failed to map buffer object (%i)", m_BufferID);
        }

        memcpy(data.get(), pBuffer, m_Size);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        SR_CORE_ASSERT(retVal, "[GlVertexBuffer]: Failed to unmap buffer object (%i)", m_BufferID);
        return std::move(data);
    }

    uint64 GlVertexBuffer::getIdentifier() const {
        return m_BufferID;
    }

}
