#include "D3D11VertexBuffer.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11VertexBuffer::D3D11VertexBuffer(const VertexBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    VertexBuffer(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Buffer(nullptr){
        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.ByteWidth = m_Size;
        bufferDesc.Usage = getD3d11BufferType(m_Usage);
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        if (m_Usage == SR_BUFFER_USAGE_DYNAMIC){
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        }
        else{
            bufferDesc.CPUAccessFlags = 0;
        }
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = m_Layout->getStride();

        if (desc.data == nullptr){
            SR_CORE_D3D11_CALL(m_Device->CreateBuffer(&bufferDesc, nullptr, &m_Buffer));
        }
        else{
            D3D11_SUBRESOURCE_DATA subResourceData;
            subResourceData.pSysMem = desc.data;
            subResourceData.SysMemPitch = 0;
            subResourceData.SysMemSlicePitch = 0;

            SR_CORE_D3D11_CALL(m_Device->CreateBuffer(&bufferDesc, &subResourceData, &m_Buffer));
        }
    }

    D3D11VertexBuffer::~D3D11VertexBuffer() {
        if (m_Buffer){
            m_Buffer->Release();
        }
    }

    void D3D11VertexBuffer::bind() {
        const uint32 offset = 0;
        const uint32 stride = m_Layout->getStride();
        m_Context->IASetVertexBuffers(0, 1, &m_Buffer, &stride, &offset);
    }

    void D3D11VertexBuffer::setData(const void *data, uint32 count){
        SR_CORE_PRECONDITION(m_Usage == SR_BUFFER_USAGE_DYNAMIC, "[VertexBuffer]: Update on buffer object (%p) requested, which has not been created with SR_BUFFER_USAGE_DYNAMIC flag!", this);
        SR_CORE_PRECONDITION(count * m_Layout->getStride() <= m_Size, "[VertexBuffer]: Update on buffer object (%p) requested, which exceeds the current buffer size (%i > %i).", this, count * m_Layout->getStride(), m_Size);

        uint32 copySize = count * m_Layout->getStride();
        m_Count = count;

        D3D11_MAPPED_SUBRESOURCE map  = { nullptr };
        SR_CORE_D3D11_CALL(m_Context->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map));
        if (map.pData == nullptr){
            SR_CORE_THROW("[D3D11VertexBuffer]: Failed to map buffer (%p)", m_Buffer);
        }
        memcpy(map.pData, data, copySize);
        m_Context->Unmap(m_Buffer, 0);
        SR_CORE_ASSERT(m_Context, "[D3D11VertexBuffer]: Failed to unmap buffer (%p)", m_Buffer);
    }

    Resource<ubyte[]> D3D11VertexBuffer::getData() const {
        Resource<ubyte[]> data(new ubyte[m_Size]);

        /*
         * Reading from a vertex buffer in D3D11 is not directly supported.
         * The only way to read from a vertex buffer is to copy the data to a staging buffer and then read from the staging buffer.
         */
        ID3D11Buffer* stagingBuffer = nullptr;
        D3D11_BUFFER_DESC desc = { 0 };
        m_Buffer->GetDesc(&desc);
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.BindFlags = 0;
        desc.MiscFlags = 0;
        SR_CORE_D3D11_CALL(m_Device->CreateBuffer(&desc, nullptr, &stagingBuffer));

        // Copy the contents of our buffer to the staging buffer
        m_Context->CopyResource(stagingBuffer, m_Buffer);

        // and copy the data
        D3D11_MAPPED_SUBRESOURCE map = { nullptr };
        SR_CORE_D3D11_CALL(m_Context->Map(stagingBuffer, 0, D3D11_MAP_READ, 0, &map));
        if (map.pData == nullptr){
            SR_CORE_THROW("[D3D11VertexBuffer]: Failed to map staging buffer (%p)", stagingBuffer);
        }
        memcpy(data.get(), map.pData, m_Size);
        m_Context->Unmap(stagingBuffer, 0);

        // Release the staging buffer
        stagingBuffer->Release();

        return std::move(data);
    }

    uint64 D3D11VertexBuffer::getIdentifier() const {
        return reinterpret_cast<uint64>(m_Buffer);
    }
}

#endif