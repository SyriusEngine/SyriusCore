#include "../../../include/SyriusCore/Context/IndexBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    IndexBuffer::IndexBuffer(const IndexBufferDesc &desc)
    : m_Count(desc.count),
    m_Type(desc.type),
    m_DataType(desc.dataType) {
        SR_CORE_PRECONDITION(desc.dataType <= SR_INT64, "Index buffer only supports integer data, type %i is not supported", desc.dataType);

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

    SR_TYPE IndexBuffer::getDataType() const {
        return m_DataType;
    }
}