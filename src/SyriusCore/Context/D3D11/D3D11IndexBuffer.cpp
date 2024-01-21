#include "D3D11IndexBuffer.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11IndexBuffer::D3D11IndexBuffer(const IndexBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    IndexBuffer(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Buffer(nullptr),
    m_DxgiDataType(getD3d11DataType(desc.dataType)){
        SR_CORE_PRECONDITION(desc.dataType == SR_UINT32 or desc.dataType == SR_UINT16, "D3D11 Index buffers only supports 16 and 32 bit integer data types");

        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.ByteWidth = m_Size;
        bufferDesc.Usage = getD3d11BufferType(m_Type);
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        if (m_Type == SR_BUFFER_USAGE_DYNAMIC){
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        }
        else{
            bufferDesc.CPUAccessFlags = 0;
        }
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = getTypeSize(m_DataType);

        D3D11_SUBRESOURCE_DATA subResourceData;
        subResourceData.pSysMem = desc.data;
        subResourceData.SysMemPitch = 0;
        subResourceData.SysMemSlicePitch = 0;

        SR_CORE_D3D11_CALL(m_Device->CreateBuffer(&bufferDesc, &subResourceData, &m_Buffer));

    }

    D3D11IndexBuffer::~D3D11IndexBuffer() {
        if (m_Buffer){
            m_Buffer->Release();
        }
    }

    void D3D11IndexBuffer::bind() {
        m_Context->IASetIndexBuffer(m_Buffer, m_DxgiDataType, 0);
    }

    void D3D11IndexBuffer::setData(const void *data){
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        SR_CORE_D3D11_CALL(m_Context->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
        memcpy(mappedResource.pData, data, m_Size);
        m_Context->Unmap(m_Buffer, 0);
    }

    uint64 D3D11IndexBuffer::getIdentifier() const {
        return reinterpret_cast<uint64>(m_Buffer);
    }
}

#endif