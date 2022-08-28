#include "../../../../include/SyriusCore/Context/ConstantBuffer.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    ConstantBuffer::ConstantBuffer(const ConstantBufferDesc &desc)
    : m_BlockName(desc.m_BlockName),
    m_Size(desc.m_Size),
    m_BindingIndex(desc.m_BindingIndex),
    m_Type(desc.m_Type){

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
