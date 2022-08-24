#include "../../../../include/SyriusCore/Context/IndexBuffer.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    IndexBuffer::IndexBuffer(const IndexBufferDesc &desc)
    : m_Count(desc.m_Count),
    m_Type(desc.m_Type),
    m_DataType(desc.m_DataType) {
        SR_CORE_PRECONDITION(desc.m_DataType > SR_INT32, "Index buffer only supports integer data")

        m_Size = m_Count * getTypeSize(m_DataType);
    }

    IndexBuffer::~IndexBuffer() {

    }

    uint32 IndexBuffer::getSize() const {
        return m_Size;
    }

    uint32 IndexBuffer::getCount() const {
        return m_Count;
    }

    SR_BUFFER_TYPE IndexBuffer::getType() const {
        return m_Type;
    }

    SR_DATA_TYPE IndexBuffer::getDataType() const {
        return m_DataType;
    }
}