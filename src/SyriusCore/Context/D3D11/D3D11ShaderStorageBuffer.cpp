#include "D3D11ShaderStorageBuffer.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11ShaderStorageBufferBase::D3D11ShaderStorageBufferBase(const ShaderStorageBufferDesc &desc, const Resource<DeviceLimits> &deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    ShaderStorageBuffer(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Buffer(nullptr),
    m_View(nullptr){
        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.ByteWidth = m_Size;
        bufferDesc.Usage = getD3d11BufferType(m_Usage);
        bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        if (m_Usage == SR_BUFFER_USAGE_DYNAMIC){
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        }
        else{
            bufferDesc.CPUAccessFlags = 0;
        }
        bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        bufferDesc.StructureByteStride = 4;

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

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
        srvDesc.BufferEx.FirstElement = 0;
        srvDesc.BufferEx.NumElements = m_Size / 4;

        SR_CORE_D3D11_CALL(m_Device->CreateShaderResourceView(m_Buffer, &srvDesc, &m_View));
    }

    D3D11ShaderStorageBufferBase::~D3D11ShaderStorageBufferBase() {
        if (m_Buffer){
            m_Buffer->Release();
        }
        if (m_View){
            m_View->Release();
        }
    }

    void D3D11ShaderStorageBufferBase::bind() {
        // D3D11 doesn't have a bind function

    }

    void D3D11ShaderStorageBufferBase::setData(const void *data, uint32 size) {
        SR_CORE_PRECONDITION(size <= m_Size, "[ShaderStorageBuffer]: supplied size (%i) exceeds the buffer size (%i)", size, m_Size);

        D3D11_MAPPED_SUBRESOURCE mappedSubresource;
        SR_CORE_D3D11_CALL(m_Context->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource));
        memcpy(mappedSubresource.pData, data, size);
        m_Context->Unmap(m_Buffer, 0);
    }

    Resource<ubyte[]> D3D11ShaderStorageBufferBase::getData() const {
        return Syrius::Resource<ubyte[]>();
    }

    uint64 D3D11ShaderStorageBufferBase::getIdentifier() const {
        return 0;
    }

    D3D11ShaderStorageBufferVertex::D3D11ShaderStorageBufferVertex(const ShaderStorageBufferDesc &desc,
                                                                   const Resource<DeviceLimits> &deviceLimits,
                                                                   ID3D11Device *device, ID3D11DeviceContext *context)
            : D3D11ShaderStorageBufferBase(desc, deviceLimits, device, context) {

    }

    D3D11ShaderStorageBufferVertex::~D3D11ShaderStorageBufferVertex() {
    }

    void D3D11ShaderStorageBufferVertex::bindShaderResource(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxShaderStorageBufferSlots(), "[ShaderStorageBuffer]: slot (%i) exceeds the maximum number of shader resource slots (%i)", slot, m_DeviceLimits->getMaxShaderStorageBufferSlots());

        m_Context->VSSetShaderResources(slot, 1, &m_View);
    }

    D3D11ShaderStorageBufferPixel::D3D11ShaderStorageBufferPixel(const ShaderStorageBufferDesc &desc,
                                                                 const Resource<DeviceLimits> &deviceLimits,
                                                                 ID3D11Device *device, ID3D11DeviceContext *context)
            : D3D11ShaderStorageBufferBase(desc, deviceLimits, device, context) {

    }

    D3D11ShaderStorageBufferPixel::~D3D11ShaderStorageBufferPixel() {
    }

    void D3D11ShaderStorageBufferPixel::bindShaderResource(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxShaderStorageBufferSlots(), "[ShaderStorageBuffer]: slot (%i) exceeds the maximum number of shader resource slots (%i)", slot, m_DeviceLimits->getMaxShaderStorageBufferSlots());

        m_Context->PSSetShaderResources(slot, 1, &m_View);
    }
}

#endif