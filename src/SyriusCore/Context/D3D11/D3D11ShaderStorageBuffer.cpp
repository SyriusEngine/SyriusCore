#include "D3D11ShaderStorageBuffer.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11ShaderStorageBuffer::D3D11ShaderStorageBuffer(const ShaderStorageBufferDesc &desc, const Resource<DeviceLimits> &deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    ShaderStorageBuffer(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Buffer(nullptr){

    }

    D3D11ShaderStorageBuffer::~D3D11ShaderStorageBuffer() {

    }

    void D3D11ShaderStorageBuffer::bind() {

    }

    void D3D11ShaderStorageBuffer::bindShaderResource(uint32 slot) {

    }

    void D3D11ShaderStorageBuffer::setData(const void *data, uint32 size) {

    }

    Resource<byte[]> D3D11ShaderStorageBuffer::getData() const {
        return Syrius::Resource<byte[]>();
    }

    uint64 D3D11ShaderStorageBuffer::getIdentifier() const {
        return 0;
    }
}

#endif