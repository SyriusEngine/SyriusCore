#include "../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ConstantBuffer::ConstantBuffer(const ConstantBufferDesc &desc, const UP<DeviceLimits>& deviceLimits):
    m_Name(desc.name),
    m_DeviceLimits(deviceLimits),
    m_Size(desc.size),
    m_Usage(desc.usage){
        SR_PRECONDITION(m_Size <= m_DeviceLimits->getMaxConstantBufferSize(), "[ConstantBuffer]: Supplied size {} exceeds device limit {}", m_Size, m_DeviceLimits->getMaxConstantBufferSize());
        SR_PRECONDITION(m_Size != 0, "[ConstantBuffer]: Size cannot be 0");
        SR_PRECONDITION(desc.name.find(' ') == std::string::npos, "[ConstantBuffer]: Name {} cannot contain spaces", desc.name);
        SR_PRECONDITION(m_Size % 16 == 0, "[ConstantBuffer]: Alignment must be 16 bytes, size is {}, add padding of size {}", m_Size, m_Size % 16);
    }

    const std::string &ConstantBuffer::getName() const {
        return m_Name;
    }

    u32 ConstantBuffer::getSize() const {
        return m_Size;
    }

    SR_BUFFER_USAGE ConstantBuffer::getUsage() const {
        return m_Usage;
    }
}
