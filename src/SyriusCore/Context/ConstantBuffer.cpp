#include "../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ConstantBuffer::ConstantBuffer(const ConstantBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits):
            m_Name(desc.name),
            m_DeviceLimits(deviceLimits),
            m_Size(desc.size),
            m_Usage(desc.usage){
        SR_CORE_PRECONDITION(m_Size <= m_DeviceLimits->getMaxConstantBufferSize(), "[ConstantBuffer]: Supplied size (%i) exceeds device limit (%i)", m_Size, m_DeviceLimits->getMaxConstantBufferSize());

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
