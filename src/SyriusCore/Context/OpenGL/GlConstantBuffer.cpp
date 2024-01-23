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

    void GlConstantBuffer::setData(const void *data) {
        glNamedBufferSubData(m_BufferID, 0, m_Size, data);
    }

    uint64 GlConstantBuffer::getIdentifier() const {
        return m_BufferID;
    }
}