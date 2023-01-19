#include "../../../../include/SyriusCore/Context/VertexBuffer.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    VertexBuffer::VertexBuffer(const VertexBufferDesc &desc)
    : m_Count(desc.m_Count),
    m_Type(desc.m_Type),
    m_Size(0),
    m_Layout(desc.m_Layout){
        SR_CORE_PRECONDITION(desc.m_Layout.isValid(), "Vertex layout cannot be null");

        m_Size = desc.m_Layout->getStride() * desc.m_Count;

    }

    VertexBuffer::~VertexBuffer() {

    }

    uint32 VertexBuffer::getSize() const {
        return m_Size;
    }

    uint32 VertexBuffer::getCount() const {
        return m_Count;
    }

    SR_BUFFER_TYPE VertexBuffer::getType() const {
        return m_Type;
    }

    const ResourceView<VertexDescription>& VertexBuffer::getVertexDescription() const {
        return m_Layout;
    }
}