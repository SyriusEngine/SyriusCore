#include "../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ConstantBuffer::ConstantBuffer(const ConstantBufferDesc &desc, const UP<DeviceLimits>& deviceLimits):
    m_DeviceLimits(deviceLimits){
        SR_PRECONDITION(desc.size <= m_DeviceLimits->getMaxConstantBufferSize(), "[ConstantBuffer]: Supplied size {} exceeds device limit {}", desc.size, m_DeviceLimits->getMaxConstantBufferSize());
        SR_PRECONDITION(desc.size  != 0, "[ConstantBuffer]: Size cannot be 0");
        SR_PRECONDITION(desc.name.find(' ') == std::string::npos, "[ConstantBuffer]: Name {} cannot contain spaces", desc.name);
        SR_PRECONDITION(desc.size  % 16 == 0, "[ConstantBuffer]: Alignment must be 16 bytes, size is {}, add padding of size {}", desc.size , desc.size  % 16);
    }
}
