#include "../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ConstantBuffer::ConstantBuffer(const ConstantBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits):
    m_Name(desc.name),
    m_DeviceLimits(deviceLimits),
    m_Size(desc.size),
    m_Usage(desc.usage){
        SR_CORE_PRECONDITION(m_Size <= m_DeviceLimits->getMaxConstantBufferSize(), "[ConstantBuffer]: Supplied size (%i) exceeds device limit (%i)", m_Size, m_DeviceLimits->getMaxConstantBufferSize());
        SR_CORE_PRECONDITION(m_Size != 0, "[ConstantBuffer]: Size cannot be %i", 0);
        SR_CORE_PRECONDITION(desc.name.find(' ') == std::string::npos, "[ConstantBuffer]: Name (%s) cannot contain spaces", desc.name.c_str());
        SR_CORE_PRECONDITION(m_Size % 16 == 0, "[ConstantBuffer]: Alignment must be 16 bytes, size is %i, add padding of size %i", m_Size, m_Size % 16);
    }

    const std::string &ConstantBuffer::getName() const {
        return m_Name;
    }

    uint32 ConstantBuffer::getSize() const {
        return m_Size;
    }

    SR_BUFFER_USAGE ConstantBuffer::getUsage() const {
        return m_Usage;
    }
}
