#include "D3D11VertexBuffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    constexpr static auto m_Name = "D3D11VertexBuffer";

    D3D11VertexBuffer::D3D11VertexBuffer(const VertexBufferDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    VertexBuffer(desc, deviceLimits),
    m_Buffer(m_Name, D3D11_BIND_VERTEX_BUFFER, 0, desc.layout->getStride() * desc.count, desc.count, desc.data, desc.usage, device, context),
    m_Device(device),
    m_Context(context){
    }

    void D3D11VertexBuffer::release() {
        m_Buffer.release();
    }

    void D3D11VertexBuffer::bind() {
        const u32 offset = 0;
        const u32 stride = m_Layout->getStride();
        ID3D11Buffer* buffer = m_Buffer.getBufferID();
        m_Context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
    }

    void D3D11VertexBuffer::setData(const void *data, u64 count){
        if (count > m_DeviceLimits->getMaxVertexCount()) {
            SR_LOG_WARNING(m_Name, "Current device supports only {} vertices, but {} requested", m_DeviceLimits->getMaxVertexCount(), count);
            return;
        }

        const Size copySize = count * m_Layout->getStride();
        m_Count = count;
        m_Buffer.setData(data, copySize);
    }

    void D3D11VertexBuffer::copyFrom(const ResourceView<VertexBuffer> &other) {
        const auto d3d11Buffer = dynamic_cast<D3D11VertexBuffer*>(other.get());
        if (!d3d11Buffer) {
            SR_LOG_WARNING(m_Name, "Failed to copy from VertexBuffer object {}", other->getIdentifier());
            return;
        }
        m_Buffer.copyFrom(d3d11Buffer->m_Buffer);
    }

    UP<UByte[]> D3D11VertexBuffer::getData() const {
        return m_Buffer.getData();
    }

    u64 D3D11VertexBuffer::getIdentifier() const {
        return reinterpret_cast<u64>(m_Buffer.getBufferID());
    }

    Size D3D11VertexBuffer::getSize() const {
        return m_Buffer.getSize();
    }

    SR_BUFFER_USAGE D3D11VertexBuffer::getUsage() const {
        return m_Buffer.getUsage();
    }

}

#endif