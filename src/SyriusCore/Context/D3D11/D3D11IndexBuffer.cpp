#include "D3D11IndexBuffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    constexpr static auto m_Name = "D3D11IndexBuffer";

    D3D11IndexBuffer::D3D11IndexBuffer(const IndexBufferDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    IndexBuffer(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Buffer(m_Name, D3D11_BIND_INDEX_BUFFER, 0, desc.count * getTypeSize(desc.dataType), desc.count, desc.data, desc.usage, device, context),
    m_DxgiDataType(getD3d11DataType(desc.dataType)){
        SR_PRECONDITION(desc.dataType == SR_UINT32 or desc.dataType == SR_UINT16, "D3D11 Index buffers only supports 16 and 32 bit integer data types, supplied type: {}", desc.dataType);
    }

    void D3D11IndexBuffer::release() {
        m_Buffer.release();
    }

    void D3D11IndexBuffer::bind() {
        const u32 offset = 0;
        m_Context->IASetIndexBuffer(m_Buffer.getBufferID(), m_DxgiDataType, offset);
    }

    void D3D11IndexBuffer::setData(const void *data, u64 count){
        if (count > m_DeviceLimits->getMaxIndexCount()) {
            SR_LOG_WARNING(m_Name, "Current device supports only {} indices, but {} requested", m_DeviceLimits->getMaxIndexCount(), count);
            return;
        }
        const Size copySize = count * getTypeSize(m_DataType);
        m_Count = count;
        m_Buffer.setData(data, copySize);
    }

    void D3D11IndexBuffer::copyFrom(const ResourceView<IndexBuffer> &other) {
        const auto d3d11Buffer = dynamic_cast<D3D11IndexBuffer*>(other.get());
        if (!d3d11Buffer) {
            SR_LOG_WARNING(m_Name, "Failed to copy from IndexBuffer object {}", other->getIdentifier());
            return;
        }
        m_Buffer.copyFrom(d3d11Buffer->m_Buffer);
    }

    UP<UByte[]> D3D11IndexBuffer::getData() const {
        return m_Buffer.getData();
    }

    u64 D3D11IndexBuffer::getIdentifier() const {
        return reinterpret_cast<u64>(m_Buffer.getBufferID());
    }

    Size D3D11IndexBuffer::getSize() const {
        return m_Buffer.getSize();
    }

    SR_BUFFER_USAGE D3D11IndexBuffer::getUsage() const {
        return m_Buffer.getUsage();
    }
}

#endif