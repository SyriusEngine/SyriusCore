#include "../../../include/SyriusCore/Context/ShaderStorageBuffer.hpp"

namespace Syrius {

    ShaderStorageBuffer::ShaderStorageBuffer(const ShaderStorageBufferDesc &desc, const UP<DeviceLimits> &deviceLimits):
    m_DeviceLimits(deviceLimits){
        SR_PRECONDITION(desc.size <= m_DeviceLimits->getMaxConstantBufferSize(), "[ShaderStorageBuffer]: Supplied size {} exceeds device limit {}", desc.size, m_DeviceLimits->getMaxShaderStorageBufferSize());
        SR_PRECONDITION(desc.size  != 0, "[ShaderStorageBuffer]: Size cannot be 0");
        SR_PRECONDITION(desc.name.find(' ') == std::string::npos, "[ShaderStorageBuffer]: Name {} cannot contain spaces", desc.name);
        SR_PRECONDITION(desc.size  % 16 == 0, "[ShaderStorageBuffer]: Alignment must be 16 bytes, size is {}, add padding of size {}", desc.size , desc.size  % 16);

    }

}
