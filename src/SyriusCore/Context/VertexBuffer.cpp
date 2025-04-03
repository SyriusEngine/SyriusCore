#include "../../../include/SyriusCore/Context/VertexBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    VertexBuffer::VertexBuffer(const VertexBufferDesc &desc, const UP<DeviceLimits>& deviceLimits):
            m_DeviceLimits(deviceLimits),
            m_Count(desc.count),
            m_Usage(desc.usage),
            m_Size(0),
            m_Layout(desc.layout){
        SR_PRECONDITION(desc.count <= deviceLimits->getMaxVertexCount(), "[VertexBuffer]: supplied vertex count {} exceeds the maximum vertex count {}", desc.count, deviceLimits->getMaxVertexCount());
        SR_PRECONDITION(desc.count > 0, "[VertexBuffer]: supplied vertex count {} must be greater than 0", desc.count);
        SR_PRECONDITION(desc.layout.get() != nullptr, "[VertexBuffer]: supplied vertex layout is null");
        

        m_Size = desc.layout->getStride() * desc.count;

    }

    VertexBuffer::~VertexBuffer() = default;

    u32 VertexBuffer::getSize() const {
        return m_Size;
    }

    u32 VertexBuffer::getCount() const {
        return m_Count;
    }

    SR_BUFFER_USAGE VertexBuffer::getUsage() const {
        return m_Usage;
    }

    const ResourceView<VertexLayout>& VertexBuffer::getLayout() const {
        return m_Layout;
    }
}