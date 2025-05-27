#include "GlShaderStorageBuffer.hpp"

namespace Syrius {

    GlShaderStorageBuffer::GlShaderStorageBuffer(const ShaderStorageBufferDesc &desc, const UP<DeviceLimits>& deviceLimits):
    ShaderStorageBuffer(desc, deviceLimits),
    m_Buffer(desc.name, GL_SHADER_STORAGE_BUFFER, desc.size, desc.count, desc.data, desc.usage){
    }

    void GlShaderStorageBuffer::release() {
        m_Buffer.release();
    }

    void GlShaderStorageBuffer::bind() {
        m_Buffer.bind();
    }

    void GlShaderStorageBuffer::bindShaderResource(u32 slot) {
        SR_PRECONDITION(slot < m_DeviceLimits->getMaxShaderStorageBufferSlots(), "[{}]: supplied slot {} exceeds the"
                            " maximum number of shader storage buffer slots {}", m_Buffer.getName(), slot,
                            m_DeviceLimits->getMaxShaderStorageBufferSlots());

        m_Buffer.bindShaderResource(slot);
    }

    void GlShaderStorageBuffer::setData(const void *data, const Size size) {
        SR_PRECONDITION(size <= m_DeviceLimits->getMaxShaderStorageBufferSize(), "[{}]: supplied size {} exceeds the maximum "
                        "shader storage buffer size {}", m_Buffer.getName(), size, m_DeviceLimits->getMaxShaderStorageBufferSize());

        m_Buffer.setData(data, size);
    }

    void GlShaderStorageBuffer::copyFrom(const ResourceView<ShaderStorageBuffer> &other) {
        const auto glBuffer = dynamic_cast<GlShaderStorageBuffer*>(other.get());
        if (!glBuffer) {
            SR_LOG_WARNING(m_Buffer.getName(), "Failed to copy from ShaderStorageBuffer object {}", other->getIdentifier());
            return;
        }
        m_Buffer.copyFrom(glBuffer->m_Buffer);
    }

    UP<UByte[]> GlShaderStorageBuffer::getData() const {
        return m_Buffer.getData();
    }

    u64 GlShaderStorageBuffer::getIdentifier() const {
        return m_Buffer.getBufferID();
    }

    const std::string& GlShaderStorageBuffer::getName() const {
        return m_Buffer.getName();
    }

    Size GlShaderStorageBuffer::getSize() const {
        return m_Buffer.getSize();
    }

    Size GlShaderStorageBuffer::getCount() const {
        return m_Buffer.getCount();
    }

    SR_BUFFER_USAGE GlShaderStorageBuffer::getUsage() const {
        return m_Buffer.getUsage();
    }
}