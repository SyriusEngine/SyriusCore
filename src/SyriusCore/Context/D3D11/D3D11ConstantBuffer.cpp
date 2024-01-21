#include "D3D11ConstantBuffer.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11ConstantBufferBase::D3D11ConstantBufferBase(const ConstantBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    ConstantBuffer(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Buffer(nullptr){

        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.ByteWidth = m_Size;
        bufferDesc.Usage = getD3d11BufferType(m_Type);
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        if (m_Type == SR_BUFFER_USAGE_DYNAMIC){
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        }
        else{
            bufferDesc.CPUAccessFlags = 0;
        }
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA subResourceData;
        subResourceData.pSysMem = desc.data;
        subResourceData.SysMemPitch = 0;
        subResourceData.SysMemSlicePitch = 0;

        SR_CORE_D3D11_CALL(m_Device->CreateBuffer(&bufferDesc, &subResourceData, &m_Buffer));
    }

    D3D11ConstantBufferBase::~D3D11ConstantBufferBase() {
        if (m_Buffer){
            m_Buffer->Release();
        }
    }

    void D3D11ConstantBufferBase::setData(const void *data) {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        SR_CORE_D3D11_CALL(m_Context->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
        memcpy(mappedResource.pData, data, m_Size);
        m_Context->Unmap(m_Buffer, 0);
    }

    uint64 D3D11ConstantBufferBase::getIdentifier() const {
        return reinterpret_cast<uint64>(m_Buffer);
    }

    D3D11ConstantBufferVertex::D3D11ConstantBufferVertex(const ConstantBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    D3D11ConstantBufferBase(desc, deviceLimits, device, context) {

    }

    void D3D11ConstantBufferVertex::bind(uint32 slot) {
        m_Context->VSSetConstantBuffers(slot, 1, &m_Buffer);
    }

    D3D11ConstantBufferPixel::D3D11ConstantBufferPixel(const ConstantBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    D3D11ConstantBufferBase(desc, deviceLimits, device, context) {

    }

    void D3D11ConstantBufferPixel::bind(uint32 slot) {
        m_Context->PSSetConstantBuffers(slot, 1, &m_Buffer);
    }

    D3D11ConstantBufferGeometry::D3D11ConstantBufferGeometry(const ConstantBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    D3D11ConstantBufferBase(desc, deviceLimits, device, context) {

    }

    void D3D11ConstantBufferGeometry::bind(uint32 slot) {
        m_Context->GSSetConstantBuffers(slot, 1, &m_Buffer);
    }
}

#endif
