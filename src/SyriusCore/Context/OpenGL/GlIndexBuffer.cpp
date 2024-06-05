#include "GlIndexBuffer.hpp"

namespace Syrius{

    GlIndexBuffer::GlIndexBuffer(const IndexBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits):
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

    void GlIndexBuffer::setData(const void *data, uint32 count) {
        SR_CORE_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlIndexBuffer]: Update on buffer object (%p) requested, which was created with SR_BUFFER_USAGE_STATIC flag!", this);
        SR_CORE_PRECONDITION(count * getTypeSize(m_DataType) <= m_Size, "[GlIndexBuffer]: Update on buffer object (%p) requested, which exceeds the current buffer size (%i > %i).", this, count * getTypeSize(m_DataType), m_Size);


        uint32 copySize = count * getTypeSize(m_DataType);
        m_Count = count;
        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_WRITE_ONLY);
        if (!pBuffer){
            SR_CORE_THROW("[GlIndexBuffer]: Failed to map buffer object (%i)", m_BufferID);
        }
        memcpy(pBuffer, data, copySize);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        SR_CORE_ASSERT(retVal, "[GlIndexBuffer]: Failed to unmap buffer object (%i)", m_BufferID);
    }

    Resource<ubyte[]> GlIndexBuffer::getData() const {
        SR_CORE_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlIndexBuffer]: Update on buffer object (%p) requested, which was created with SR_BUFFER_USAGE_STATIC flag!", this);

        auto data = createResource<ubyte[]>(m_Size);
        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_READ_ONLY);
        if (!pBuffer){
            SR_CORE_THROW("[GlIndexBuffer]: Failed to map buffer object (%i)", m_BufferID);
        }

        memcpy(data.get(), pBuffer, m_Size);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        SR_CORE_ASSERT(retVal, "[GlIndexBuffer]: Failed to unmap buffer object (%i)", m_BufferID);
        return std::move(data);
    }

    uint64 GlIndexBuffer::getIdentifier() const {
        return m_BufferID;
    }

}