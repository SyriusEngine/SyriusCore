#include "D3D11ShaderStorageBuffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius {

    D3D11ShaderStorageBufferBase::D3D11ShaderStorageBufferBase(const ShaderStorageBufferDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context):
    ShaderStorageBuffer(desc, deviceLimits),
    m_Buffer(desc.name, D3D11_BIND_SHADER_RESOURCE, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, desc.size, desc.count, desc.data, desc.usage, device, context),
    m_Device(device),
    m_Context(context){
        create_srv();
    }

    D3D11ShaderStorageBufferBase::~D3D11ShaderStorageBufferBase() {
        release_srv();
    }

    void D3D11ShaderStorageBufferBase::release() {
        release_srv();
        m_Buffer.release();
    }

    void D3D11ShaderStorageBufferBase::bind() {

    }

    void D3D11ShaderStorageBufferBase::setData(const void *data, const Size size) {
        SR_PRECONDITION(size <= m_DeviceLimits->getMaxShaderStorageBufferSize(), "[{}]: supplied size {} exceeds the maximum constant buffer size {}", m_Buffer.getName(), size, m_DeviceLimits->getMaxShaderStorageBufferSize());

        m_Buffer.setData(data, size);
    }

    void D3D11ShaderStorageBufferBase::copyFrom(const ResourceView<ShaderStorageBuffer> &other) {
        const auto d3d11Buffer = dynamic_cast<D3D11ShaderStorageBufferBase*>(other.get());
        if (!d3d11Buffer) {
            SR_LOG_WARNING(m_Buffer.getName(), "Failed to copy from ShaderStorageBuffer object {}", other->getIdentifier());
            return;
        }
        m_Buffer.copyFrom(d3d11Buffer->m_Buffer);
    }

    UP<UByte[]> D3D11ShaderStorageBufferBase::getData() const {
        return m_Buffer.getData();
    }

    u64 D3D11ShaderStorageBufferBase::getIdentifier() const {
        return reinterpret_cast<u64>(m_Buffer.getBufferID());
    }

    const std::string& D3D11ShaderStorageBufferBase::getName() const {
        return m_Buffer.getName();
    }

    Size D3D11ShaderStorageBufferBase::getSize() const {
        return m_Buffer.getSize();
    }

    Size D3D11ShaderStorageBufferBase::getCount() const {
        return m_Buffer.getCount();
    }

    SR_BUFFER_USAGE D3D11ShaderStorageBufferBase::getUsage() const {
        return m_Buffer.getUsage();
    }

    void D3D11ShaderStorageBufferBase::create_srv() {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Format = DXGI_FORMAT_UNKNOWN; // We don't know the format of the data, also does not matter
        srvDesc.Buffer.ElementOffset = 0;
        srvDesc.Buffer.NumElements = m_Buffer.getCount();

        SR_CORE_D3D11_CALL(m_Device->CreateShaderResourceView(m_Buffer.getBufferID(), &srvDesc, &m_ShaderResourceView));
    }

    void D3D11ShaderStorageBufferBase::release_srv() const {
        if (m_ShaderResourceView) {
            m_ShaderResourceView->Release();
        }
    }

    D3D11ShaderStorageBufferVertex::D3D11ShaderStorageBufferVertex(const ShaderStorageBufferDesc &desc, const UP<DeviceLimits> &deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    D3D11ShaderStorageBufferBase(desc, deviceLimits, device, context){

    }

    void D3D11ShaderStorageBufferVertex::bindShaderResource(const u32 slot) {
        m_Context->VSSetShaderResources(slot, 1, &m_ShaderResourceView);
    }

    D3D11ShaderStorageBufferFragment::D3D11ShaderStorageBufferFragment(const ShaderStorageBufferDesc &desc, const UP<DeviceLimits> &deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    D3D11ShaderStorageBufferBase(desc, deviceLimits, device, context){

    }

    void D3D11ShaderStorageBufferFragment::bindShaderResource(const u32 slot) {
        m_Context->PSSetShaderResources(slot, 1, &m_ShaderResourceView);
    }

    D3D11ShaderStorageBufferGeometry::D3D11ShaderStorageBufferGeometry(const ShaderStorageBufferDesc &desc, const UP<DeviceLimits> &deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    D3D11ShaderStorageBufferBase(desc, deviceLimits, device, context){

    }

    void D3D11ShaderStorageBufferGeometry::bindShaderResource(const u32 slot) {
        m_Context->GSSetShaderResources(slot, 1, &m_ShaderResourceView);
    }



}

#endif
