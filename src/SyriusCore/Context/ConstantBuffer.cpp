#include "../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ConstantBuffer::ConstantBuffer(const ConstantBufferDesc &desc)
    : m_BlockName(desc.name),
    m_Size(desc.size),
    m_BindingIndex(desc.slot),
    m_Type(desc.type){
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

    uint32 ConstantBuffer::getBindingIndex() const {
        return m_BindingIndex;
    }

    SR_BUFFER_TYPE ConstantBuffer::getBufferType() const {
        return m_Type;
    }
}
