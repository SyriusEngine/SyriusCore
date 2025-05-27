#include "GlVertexBuffer.hpp"

namespace Syrius{

    constexpr static auto m_Name = "GlVertexBuffer";

    GlVertexBuffer::GlVertexBuffer(const VertexBufferDesc &desc, const UP<DeviceLimits>& deviceLimits):
    VertexBuffer(desc, deviceLimits),
    m_Buffer(m_Name, GL_ARRAY_BUFFER, desc.layout->getStride() * desc.count, desc.count, desc.data, desc.usage){
    }

    void GlVertexBuffer::release() {
        m_Buffer.release();
    }

    void GlVertexBuffer::bind() {
        m_Buffer.bind();
    }

    void GlVertexBuffer::setData(const void *data, const u64 count) {
        if (count > m_DeviceLimits->getMaxVertexCount()) {
            SR_LOG_WARNING(m_Name, "Current device supports only {} vertices, but {} requested", m_DeviceLimits->getMaxVertexCount(), count);
            return;
        }

        const Size copySize = count * m_Layout->getStride();
        m_Count = count;
        m_Buffer.setData(data, copySize);
    }

    void GlVertexBuffer::copyFrom(const ResourceView<VertexBuffer> &other) {
        const auto glBuffer = dynamic_cast<GlVertexBuffer*>(other.get());
        if (!glBuffer) {
            SR_LOG_WARNING(m_Name, "Failed to copy from VertexBuffer object {}", other->getIdentifier());
            return;
        }
        m_Buffer.copyFrom(glBuffer->m_Buffer);
    }

    UP<UByte[]> GlVertexBuffer::getData() const {
        return m_Buffer.getData();
    }

    u64 GlVertexBuffer::getIdentifier() const {
        return m_Buffer.getBufferID();
    }

    Size GlVertexBuffer::getSize() const {
        return m_Buffer.getSize();
    }

    SR_BUFFER_USAGE GlVertexBuffer::getUsage() const {
        return m_Buffer.getUsage();
    }

}
