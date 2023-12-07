#include "../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ConstantBuffer::ConstantBuffer(const ConstantBufferDesc &desc)
    : m_BlockName(desc.name),
    m_Size(desc.size),
    m_Type(desc.usage){
        SR_CORE_PRECONDITION(desc.size > 0, "Size %i is not greater than 0", desc.size);
        SR_CORE_PRECONDITION(desc.size % 16 == 0, "Constant buffer size must be aligned to 16 bytes, size: %i", desc.size);
        SR_CORE_PRECONDITION(desc.data != nullptr, "Constant buffer data must not be null");
        SR_CORE_PRECONDITION(!desc.name.empty(), "Constant buffer block name must not be empty");
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
