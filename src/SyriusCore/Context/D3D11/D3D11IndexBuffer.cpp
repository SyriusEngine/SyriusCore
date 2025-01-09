#include "D3D11IndexBuffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11IndexBuffer::D3D11IndexBuffer(const IndexBufferDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    IndexBuffer(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Buffer(nullptr),
    m_DxgiDataType(getD3d11DataType(desc.dataType)){
        SR_PRECONDITION(desc.dataType == SR_UINT32 or desc.dataType == SR_UINT16, "D3D11 Index buffers only supports 16 and 32 bit integer data types, supplied type: %i", desc.dataType);

        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.ByteWidth = m_Size;
        bufferDesc.Usage = getD3d11BufferType(m_Usage);
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        if (m_Usage == SR_BUFFER_USAGE_DYNAMIC){
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        }
        else{
            bufferDesc.CPUAccessFlags = 0;
        }
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = getTypeSize(m_DataType);

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

    D3D11IndexBuffer::~D3D11IndexBuffer() {
        if (m_Buffer){
            m_Buffer->Release();
        }
    }

    void D3D11IndexBuffer::bind() {
        m_Context->IASetIndexBuffer(m_Buffer, m_DxgiDataType, 0);
    }

    void D3D11IndexBuffer::setData(const void *data, u32 count){
        SR_PRECONDITION(m_Usage == SR_BUFFER_USAGE_DYNAMIC, "[IndexBuffer]: Update on buffer object (%p) requested, which has not been created with SR_BUFFER_USAGE_DYNAMIC flag!", this);
        SR_PRECONDITION(count * getTypeSize(m_DataType) <= m_Size, "[IndexBuffer]: Update on buffer object (%p) requested, which exceeds the current buffer size (%i > %i).", this, count * getTypeSize(m_DataType), m_Size);


        u32 copySize = count * getTypeSize(m_DataType);
        m_Count = count;

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        SR_CORE_D3D11_CALL(m_Context->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
        memcpy(mappedResource.pData, data, copySize);
        m_Context->Unmap(m_Buffer, 0);
    }

    void D3D11IndexBuffer::copyFrom(const ResourceView<IndexBuffer> &other) {
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[D3D11IndexBuffer]: Update on buffer object (%p) requested, which was created with SR_BUFFER_USAGE_STATIC flag!", this);

        if (m_Size >= other->getSize()){
            auto otherBuffer = reinterpret_cast<ID3D11Buffer*>(other->getIdentifier());
            SR_ASSERT(otherBuffer, "[D3D11IndexBuffer]: Copy from buffer object (%p) requested, which is invalid!", other.get());
            m_Context->CopyResource(m_Buffer, otherBuffer);
        }
        else{
            SR_LOG_WARNING("[D3D11IndexBuffer]", "Copy from buffer object (%p) requested, which exceeds the current "
                            "buffer size (%i > %i)", this, other->getSize(), m_Size);
        }
    }

    UP<UByte[]> D3D11IndexBuffer::getData() const {
        auto data = createUP<UByte[]>(m_Size);

        /*
         * Reading from an index buffer in D3D11 is not directly supported.
         * The only way to read from an index buffer is to copy the data to a staging buffer and then read from the staging buffer.
         */
        ID3D11Buffer* stagingBuffer = nullptr;
        D3D11_BUFFER_DESC bufferDesc;
        m_Buffer->GetDesc(&bufferDesc);
        bufferDesc.Usage = D3D11_USAGE_STAGING;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        bufferDesc.BindFlags = 0;
        bufferDesc.MiscFlags = 0;
        SR_CORE_D3D11_CALL(m_Device->CreateBuffer(&bufferDesc, nullptr, &stagingBuffer));

        // Copy the data from the index buffer to the staging buffer
        m_Context->CopyResource(stagingBuffer, m_Buffer);

        // and copy the data from the staging buffer to the data array
        D3D11_MAPPED_SUBRESOURCE map = { nullptr };
        SR_CORE_D3D11_CALL(m_Context->Map(stagingBuffer, 0, D3D11_MAP_READ, 0, &map));

        SR_LOG_THROW_IF_FALSE(map.pData != nullptr, "D3D11IndexBuffer", "Failed to map staging buffer (%p)", stagingBuffer);

        memcpy(data.get(), map.pData, m_Size);
        m_Context->Unmap(stagingBuffer, 0);

        // Release the staging buffer
        stagingBuffer->Release();

        return std::move(data);
    }

    u64 D3D11IndexBuffer::getIdentifier() const {
        return reinterpret_cast<u64>(m_Buffer);
    }
}

#endif