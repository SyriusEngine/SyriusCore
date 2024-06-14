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
        bufferDesc.Usage = getD3d11BufferType(m_Usage);
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        if (m_Usage == SR_BUFFER_USAGE_DYNAMIC){
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        }
        else{
            bufferDesc.CPUAccessFlags = 0;
        }
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;

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

    D3D11ConstantBufferBase::~D3D11ConstantBufferBase() {
        if (m_Buffer){
            m_Buffer->Release();
        }
    }

    void D3D11ConstantBufferBase::setData(const void *data, uint64 size) {
        SR_CORE_PRECONDITION(m_Usage == SR_BUFFER_USAGE_DYNAMIC, "[ConstantBuffer]: Update on buffer object (%p) requested, which has not been created with SR_BUFFER_USAGE_DYNAMIC flag!", this);
        SR_CORE_PRECONDITION(size <= m_Size, "[ConstantBuffer]: Update on buffer object (%p) requested, which exceeds the current buffer size (%i > %i).", this, size, m_Size);

        D3D11_MAPPED_SUBRESOURCE map = { nullptr };
        SR_CORE_D3D11_CALL(m_Context->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map));
        if (map.pData == nullptr){
            SR_CORE_THROW("[ConstantBuffer]: Failed to map buffer (%p)", m_Buffer);
        }
        memcpy(map.pData, data, size);
        m_Context->Unmap(m_Buffer, 0);
    }

    void D3D11ConstantBufferBase::copyFrom(const ResourceView<ConstantBuffer> &other) {
        SR_CORE_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[D3D11ConstantBuffer]: Update on buffer object (%p) requested, which was created with SR_BUFFER_USAGE_STATIC flag!", this);

        if (m_Size >= other->getSize()){
            auto otherBuffer = reinterpret_cast<ID3D11Buffer*>(other->getIdentifier());
            SR_CORE_ASSERT(otherBuffer, "[D3D11ConstantBuffer]: Copy from buffer object (%p) requested, which has no valid identifier!", other.get());
            m_Context->CopyResource(m_Buffer, otherBuffer);
        }
        else{
            SR_CORE_WARNING("[D3D11ConstantBuffer]: Copy from buffer object (%p) requested, which exceeds the current "
                            "buffer size (%i > %i)", this, other->getSize(), m_Size);
        }
    }

    Resource<ubyte[]> D3D11ConstantBufferBase::getData() const {
        auto data = createResource<ubyte[]>(m_Size);

        /*
         * Reading from a constant buffer in D3D11 is not directly supported.
         * The only way to read from a constant buffer is to copy the data to a staging buffer and then read from the staging buffer.
         */
        ID3D11Buffer* stagingBuffer = nullptr;
        D3D11_BUFFER_DESC desc = { 0 };
        m_Buffer->GetDesc(&desc);
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.BindFlags = 0;
        desc.MiscFlags = 0;
        SR_CORE_D3D11_CALL(m_Device->CreateBuffer(&desc, nullptr, &stagingBuffer));

        // Copy the data from the constant buffer to the staging buffer
        m_Context->CopyResource(stagingBuffer, m_Buffer);

        // copy the data
        D3D11_MAPPED_SUBRESOURCE map = { nullptr };
        SR_CORE_D3D11_CALL(m_Context->Map(stagingBuffer, 0, D3D11_MAP_READ, 0, &map));
        if (map.pData == nullptr){
            SR_CORE_THROW("[ConstantBuffer]: Failed to map buffer (%p)", stagingBuffer);
        }
        memcpy(data.get(), map.pData, m_Size);
        m_Context->Unmap(stagingBuffer, 0);

        // Release the staging buffer
        stagingBuffer->Release();

        return std::move(data);
    }

    uint64 D3D11ConstantBufferBase::getIdentifier() const {
        return reinterpret_cast<uint64>(m_Buffer);
    }

    D3D11ConstantBufferVertex::D3D11ConstantBufferVertex(const ConstantBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    D3D11ConstantBufferBase(desc, deviceLimits, device, context) {

    }

    void D3D11ConstantBufferVertex::bind(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxConstantBufferSlots(), "[ConstantBuffer]: supplied slot (%i) exceeds the maximum number of constant buffer slots (%i)", slot, m_DeviceLimits->getMaxConstantBufferSlots());

        m_Context->VSSetConstantBuffers(slot, 1, &m_Buffer);
    }

    D3D11ConstantBufferPixel::D3D11ConstantBufferPixel(const ConstantBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    D3D11ConstantBufferBase(desc, deviceLimits, device, context) {

    }

    void D3D11ConstantBufferPixel::bind(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxConstantBufferSlots(), "[ConstantBuffer]: supplied slot (%i) exceeds the maximum number of constant buffer slots (%i)", slot, m_DeviceLimits->getMaxConstantBufferSlots());

        m_Context->PSSetConstantBuffers(slot, 1, &m_Buffer);
    }

    D3D11ConstantBufferGeometry::D3D11ConstantBufferGeometry(const ConstantBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    D3D11ConstantBufferBase(desc, deviceLimits, device, context) {

    }

    void D3D11ConstantBufferGeometry::bind(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxConstantBufferSlots(), "[ConstantBuffer]: supplied slot (%i) exceeds the maximum number of constant buffer slots (%i)", slot, m_DeviceLimits->getMaxConstantBufferSlots());

        m_Context->GSSetConstantBuffers(slot, 1, &m_Buffer);
    }
}

#endif
