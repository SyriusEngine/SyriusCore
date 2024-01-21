#include "../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ConstantBuffer::ConstantBuffer(const ConstantBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits):
    m_BlockName(desc.name),
    m_DeviceLimits(deviceLimits),
    m_Size(desc.size),
    m_Type(desc.usage){
        SR_CORE_PRECONDITION(m_Size <= m_DeviceLimits->getMaxConstantBufferSize(), "[ConstantBuffer]: Supplied size (%i) exceeds device limit (%i)", m_Size, m_DeviceLimits->getMaxConstantBufferSize());

    }

    const std::string &ConstantBuffer::getBlockName() const {
        return m_BlockName;
    }

    uint32 ConstantBuffer::getSize() const {
        return m_Size;
    }

    SR_BUFFER_USAGE ConstantBuffer::getBufferType() const {
        return m_Type;
    }
}
