#include "GlConstantBuffer.hpp"

namespace Syrius{

    GlConstantBuffer::GlConstantBuffer(const ConstantBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits):
    ConstantBuffer(desc, deviceLimits),
    m_BufferID(0){
        glCreateBuffers(1, &m_BufferID);
        glNamedBufferData(m_BufferID, m_Size, desc.data, getGlBufferUsage(desc.usage));
    }

    GlConstantBuffer::~GlConstantBuffer() {
        glDeleteBuffers(1, &m_BufferID);
    }

    void GlConstantBuffer::bind(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxConstantBufferSlots(), "[ConstantBuffer]: supplied slot (%i) exceeds the maximum number of constant buffer slots (%i)", slot, m_DeviceLimits->getMaxConstantBufferSlots());

        glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_BufferID);
    }

    void GlConstantBuffer::setData(const void *data, uint64 size) {
        SR_CORE_PRECONDITION(m_Usage == SR_BUFFER_USAGE_DYNAMIC, "[ConstantBuffer]: Update on buffer object (%p) requested, which has not been created with SR_BUFFER_USAGE_DYNAMIC flag!", this);
        SR_CORE_PRECONDITION(size <= m_Size, "[ConstantBuffer]: Update on buffer object (%p) requested, which exceeds the current buffer size (%i > %i).", this, size, m_Size);

        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_WRITE_ONLY);
        if (!pBuffer){
            SR_CORE_THROW("[GlConstantBuffer]: Failed to map buffer object (%i)", m_BufferID);
        }
        memcpy(pBuffer, data, size);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        SR_CORE_ASSERT(retVal, "[GlConstantBuffer]: Failed to unmap buffer object (%i)", m_BufferID);
    }

    Resource<ubyte[]> GlConstantBuffer::getData() const {
        auto data = createResource<ubyte[]>(m_Size);
        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_READ_ONLY);
        if (!pBuffer){
            SR_CORE_THROW("[GlConstantBuffer]: Failed to map buffer object (%i)", m_BufferID);
        }

        memcpy(data.get(), pBuffer, m_Size);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        SR_CORE_ASSERT(retVal, "[GlConstantBuffer]: Failed to unmap buffer object (%i)", m_BufferID);
        return std::move(data);
    }

    uint64 GlConstantBuffer::getIdentifier() const {
        return m_BufferID;
    }
}