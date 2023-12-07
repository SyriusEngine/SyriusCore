#include "../../../include/SyriusCore/Context/VertexBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    VertexBuffer::VertexBuffer(const VertexBufferDesc &desc)
    : m_Count(desc.count),
    m_Type(desc.usage),
    m_Size(0),
    m_Layout(desc.layout){
        SR_CORE_PRECONDITION(desc.layout.isValid(), "Vertex layout cannot be null");
        SR_CORE_PRECONDITION(desc.count > 0, "Vertex count must be greater than 0");

        m_Size = desc.layout->getStride() * desc.count;

    }

    VertexBuffer::~VertexBuffer() {

    }

    uint32 VertexBuffer::getSize() const {
        return m_Size;
    }

    uint32 VertexBuffer::getCount() const {
        return m_Count;
    }

    SR_BUFFER_USAGE VertexBuffer::getType() const {
        return m_Type;
    }

    const ResourceView<VertexLayout>& VertexBuffer::getVertexDescription() const {
        return m_Layout;
    }
}