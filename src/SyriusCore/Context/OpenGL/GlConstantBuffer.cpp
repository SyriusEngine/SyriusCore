#include "GlConstantBuffer.hpp"

namespace Syrius{

    GlConstantBuffer::GlConstantBuffer(const ConstantBufferDesc &desc, const UP<DeviceLimits>& deviceLimits):
    ConstantBuffer(desc, deviceLimits),
    m_BufferID(0){
        glCreateBuffers(1, &m_BufferID);
        glNamedBufferData(m_BufferID, m_Size, desc.data, getGlBufferType(desc.usage));
    }

    GlConstantBuffer::~GlConstantBuffer() {
        glDeleteBuffers(1, &m_BufferID);
    }

    void GlConstantBuffer::bind(u32 slot) {
        SR_PRECONDITION(slot < m_DeviceLimits->getMaxConstantBufferSlots(), "[GlConstantBuffer]: supplied slot {} exceeds the maximum number of constant buffer slots {}", slot, m_DeviceLimits->getMaxConstantBufferSlots());
        SR_PRECONDITION(m_Size != 0, "[GlConstantBuffer]: Buffer object {} has size 0, which cannot be bound!", m_BufferID);

        glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_BufferID);
    }

    void GlConstantBuffer::setData(const void *data, u64 size) {
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlConstantBuffer]: Update on buffer object {} requested, which was created with SR_BUFFER_USAGE_STATIC flag!", m_BufferID);
        SR_PRECONDITION(size <= m_Size, "[GlConstantBuffer]: Update on buffer object requested {}, which exceeds the current buffer size ({} > {}).", m_BufferID, size, m_Size);

        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_WRITE_ONLY);

        if (!pBuffer) {
            SR_LOG_WARNING("GlConstantBuffer", "Failed to map buffer object (%i)", m_BufferID)
            return;
        }

        memcpy(pBuffer, data, size);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        if (!retVal) {
            SR_LOG_WARNING("GlConstantBuffer", "Failed to unmap buffer object (%i)", m_BufferID)
        }
    }

    void GlConstantBuffer::copyFrom(const ResourceView<ConstantBuffer> &other) {
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlConstantBuffer]: Update on buffer object {} requested, which was created with SR_BUFFER_USAGE_STATIC flag!", m_BufferID);

        if (m_Size < other->getSize()){
            SR_LOG_WARNING("GlConstantBuffer", "Copy from buffer object {} requested, which exceeds the current buffer size ({} > {}).", m_BufferID, other->getSize(), m_Size);
        }
        auto copySize = std::min(m_Size, other->getSize());
        glCopyNamedBufferSubData(other->getIdentifier(), m_BufferID, 0, 0, copySize);
    }

    UP<UByte[]> GlConstantBuffer::getData() const {
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlConstantBuffer]: Update on buffer object {} requested, which was created with SR_BUFFER_USAGE_STATIC flag!", m_BufferID);

        auto data = createUP<UByte[]>(m_Size);
        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_READ_ONLY);

        if (!pBuffer) {
            SR_LOG_WARNING("GlConstantBuffer", "Failed to map buffer object {}", m_BufferID);
            return std::move(data);
        }

        memcpy(data.get(), pBuffer, m_Size);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        if (retVal == GL_FALSE) {
            SR_LOG_WARNING("GlConstantBuffer", "Failed to unmap buffer object {}", m_BufferID);
        }
        return std::move(data);
    }

    u64 GlConstantBuffer::getIdentifier() const {
        return m_BufferID;
    }
}