#include "GlConstantBuffer.hpp"

namespace Syrius{

    GlConstantBuffer::GlConstantBuffer(const ConstantBufferDesc &desc, const UP<DeviceLimits>& deviceLimits):
    ConstantBuffer(desc, deviceLimits),
    m_Buffer(desc.name, GL_UNIFORM_BUFFER, desc.size, desc.count, desc.data, desc.usage){
    }

    void GlConstantBuffer::release() {
        m_Buffer.release();
    }

    void GlConstantBuffer::bind() {
        m_Buffer.bind();
    }

    void GlConstantBuffer::bindShaderResource(u32 slot) {
        SR_PRECONDITION(slot < m_DeviceLimits->getMaxConstantBufferSlots(), "[{}]: supplied slot {} exceeds the maximum number of constant buffer slots {}", m_Buffer.getName(), slot, m_DeviceLimits->getMaxConstantBufferSlots());

        m_Buffer.bindShaderResource(slot);
    }

    void GlConstantBuffer::setData(const void *data, const Size size) {
        SR_PRECONDITION(size <= m_DeviceLimits->getMaxConstantBufferSize(), "[{}]: supplied size {} exceeds the maximum constant buffer size {}", m_Buffer.getName(), size, m_DeviceLimits->getMaxConstantBufferSize());

        m_Buffer.setData(data, size);
    }

    void GlConstantBuffer::copyFrom(const ResourceView<ConstantBuffer> &other) {
        const auto glBuffer = dynamic_cast<GlConstantBuffer*>(other.get());
        if (!glBuffer) {
            SR_LOG_WARNING(m_Buffer.getName(), "Failed to copy from ConstantBuffer object {}", other->getIdentifier());
            return;
        }
        m_Buffer.copyFrom(glBuffer->m_Buffer);
    }

    UP<UByte[]> GlConstantBuffer::getData() const {
        return m_Buffer.getData();
    }

    u64 GlConstantBuffer::getIdentifier() const {
        return m_Buffer.getBufferID();
    }

    const std::string& GlConstantBuffer::getName() const {
        return m_Buffer.getName();
    }

    Size GlConstantBuffer::getSize() const {
        return m_Buffer.getSize();
    }

    SR_BUFFER_USAGE GlConstantBuffer::getUsage() const {
        return m_Buffer.getUsage();
    }
}