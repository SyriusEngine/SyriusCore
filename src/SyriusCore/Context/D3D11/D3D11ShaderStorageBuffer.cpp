#include "D3D11ShaderStorageBuffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius {

    D3D11ShaderStorageBuffer::D3D11ShaderStorageBuffer(const ShaderStorageBufferDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device* device, ID3D11DeviceContext* context):
    ShaderStorageBuffer(desc, deviceLimits){
    }

    void D3D11ShaderStorageBuffer::release() {
        // m_Buffer.release();
    }

    void D3D11ShaderStorageBuffer::bind() {
        // m_Buffer.bind();
    }

    void D3D11ShaderStorageBuffer::bindShaderResource(u32 slot) {

    }

    void D3D11ShaderStorageBuffer::setData(const void *data, const Size size) {
    }

    void D3D11ShaderStorageBuffer::copyFrom(const ResourceView<ShaderStorageBuffer> &other) {

    }

    UP<UByte[]> D3D11ShaderStorageBuffer::getData() const {
        return nullptr;
    }

    u64 D3D11ShaderStorageBuffer::getIdentifier() const {
        return 0;
    }

    const std::string& D3D11ShaderStorageBuffer::getName() const {
        return "";
    }

    Size D3D11ShaderStorageBuffer::getSize() const {
        return 0;
    }

    SR_BUFFER_USAGE D3D11ShaderStorageBuffer::getUsage() const {
        return SR_BUFFER_USAGE_DEFAULT;
    }

}

#endif
