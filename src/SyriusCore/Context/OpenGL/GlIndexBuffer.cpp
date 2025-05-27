#include "GlIndexBuffer.hpp"

namespace Syrius{

    constexpr static auto m_Name = "GlIndexBuffer";

    GlIndexBuffer::GlIndexBuffer(const IndexBufferDesc &desc, const UP<DeviceLimits>& deviceLimits):
    IndexBuffer(desc, deviceLimits),
    m_Buffer(m_Name, GL_ELEMENT_ARRAY_BUFFER, desc.count * getTypeSize(desc.dataType), desc.count, desc.data, desc.usage){
    }

    void GlIndexBuffer::release() {
        m_Buffer.release();
    }

    void GlIndexBuffer::bind() {
        m_Buffer.bind();
    }

    void GlIndexBuffer::setData(const void *data, const u64 count) {
        if (count > m_DeviceLimits->getMaxIndexCount()) {
            SR_LOG_WARNING(m_Name, "Current device supports only {} indices, but {} requested", m_DeviceLimits->getMaxIndexCount(), count);
            return;
        }
        const Size copySize = count * getTypeSize(m_DataType);
        m_Count = count;
        m_Buffer.setData(data, copySize);
    }

    void GlIndexBuffer::copyFrom(const ResourceView<IndexBuffer> &other) {
        const auto glBuffer = dynamic_cast<GlIndexBuffer*>(other.get());
        if (!glBuffer) {
            SR_LOG_WARNING(m_Name, "Failed to copy from IndexBuffer object {}", other->getIdentifier());
            return;
        }
        m_Buffer.copyFrom(glBuffer->m_Buffer);
    }

    UP<UByte[]> GlIndexBuffer::getData() const {
        return m_Buffer.getData();
    }

    u64 GlIndexBuffer::getIdentifier() const {
        return m_Buffer.getBufferID();
    }

    Size GlIndexBuffer::getSize() const {
        return m_Buffer.getSize();
    }

    SR_BUFFER_USAGE GlIndexBuffer::getUsage() const {
        return m_Buffer.getUsage();
    }

}