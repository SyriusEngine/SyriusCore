#include "D3D11ConstantBuffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11ConstantBufferBase::D3D11ConstantBufferBase(const ConstantBufferDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    ConstantBuffer(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Buffer(desc.name, D3D11_BIND_CONSTANT_BUFFER, desc.size, desc.data, desc.usage, device, context){
    }

    void D3D11ConstantBufferBase::release() {
        m_Buffer.release();
    }

    void D3D11ConstantBufferBase::bind() {
        // Not necessary in D3D11
    }

    void D3D11ConstantBufferBase::setData(const void *data, u64 size) {
        SR_PRECONDITION(size <= m_DeviceLimits->getMaxConstantBufferSize(), "[{}]: supplied size {} exceeds the maximum constant buffer size {}", m_Buffer.getName(), size, m_DeviceLimits->getMaxConstantBufferSize());

        m_Buffer.setData(data, size);
    }

    void D3D11ConstantBufferBase::copyFrom(const ResourceView<ConstantBuffer> &other) {
        const auto d3d11Buffer = dynamic_cast<D3D11ConstantBufferBase*>(other.get());
        if (!d3d11Buffer) {
            SR_LOG_WARNING(m_Buffer.getName(), "Failed to copy from ConstantBuffer object {}", other->getIdentifier());
            return;
        }
        m_Buffer.copyFrom(d3d11Buffer->m_Buffer);
    }

    UP<UByte[]> D3D11ConstantBufferBase::getData() const {
        return m_Buffer.getData();
    }

    u64 D3D11ConstantBufferBase::getIdentifier() const {
        return reinterpret_cast<u64>(m_Buffer.getBufferID());
    }

    const std::string& D3D11ConstantBufferBase::getName() const {
        return m_Buffer.getName();
    }

    Size D3D11ConstantBufferBase::getSize() const {
        return m_Buffer.getSize();
    }

    SR_BUFFER_USAGE D3D11ConstantBufferBase::getUsage() const {
        return m_Buffer.getUsage();
    }

    D3D11ConstantBufferVertex::D3D11ConstantBufferVertex(const ConstantBufferDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    D3D11ConstantBufferBase(desc, deviceLimits, device, context) {

    }

    void D3D11ConstantBufferVertex::bindShaderResource(u32 slot) {
        SR_PRECONDITION(slot < m_DeviceLimits->getMaxConstantBufferSlots(), "[ConstantBuffer]: supplied slot ({}) exceeds the maximum number of constant buffer slots ({})", slot, m_DeviceLimits->getMaxConstantBufferSlots());

        ID3D11Buffer* buffer = m_Buffer.getBufferID();
        m_Context->VSSetConstantBuffers(slot, 1, &buffer);
    }

    D3D11ConstantBufferPixel::D3D11ConstantBufferPixel(const ConstantBufferDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    D3D11ConstantBufferBase(desc, deviceLimits, device, context) {

    }

    void D3D11ConstantBufferPixel::bindShaderResource(u32 slot) {
        SR_PRECONDITION(slot < m_DeviceLimits->getMaxConstantBufferSlots(), "[ConstantBuffer]: supplied slot ({}) exceeds the maximum number of constant buffer slots ({})", slot, m_DeviceLimits->getMaxConstantBufferSlots());

        ID3D11Buffer* buffer = m_Buffer.getBufferID();
        m_Context->PSSetConstantBuffers(slot, 1, &buffer);
    }

    D3D11ConstantBufferGeometry::D3D11ConstantBufferGeometry(const ConstantBufferDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    D3D11ConstantBufferBase(desc, deviceLimits, device, context) {

    }

    void D3D11ConstantBufferGeometry::bindShaderResource(u32 slot) {
        SR_PRECONDITION(slot < m_DeviceLimits->getMaxConstantBufferSlots(), "[ConstantBuffer]: supplied slot ({}) exceeds the maximum number of constant buffer slots ({})", slot, m_DeviceLimits->getMaxConstantBufferSlots());

        ID3D11Buffer* buffer = m_Buffer.getBufferID();
        m_Context->GSSetConstantBuffers(slot, 1, &buffer);
    }
}

#endif
