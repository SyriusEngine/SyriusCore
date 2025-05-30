#include "../../../include/SyriusCore/Context/IndexBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    IndexBuffer::IndexBuffer(const IndexBufferDesc &desc, const UP<DeviceLimits>& deviceLimits):
    m_DeviceLimits(deviceLimits),
    m_Count(desc.count),
    m_DataType(desc.dataType) {
        SR_PRECONDITION(desc.dataType >= SR_UINT8 and desc.dataType <= SR_INT32, "[IndexBuffer]: Index buffer only supports integer data, type {} is not supported", desc.dataType);
        SR_PRECONDITION(desc.count > 0, "[IndexBuffer]: Index buffer count must be greater than {}", 0);
        SR_PRECONDITION(desc.count <= m_DeviceLimits->getMaxIndexCount(), "[IndexBuffer]: Supplied index count {} exceeds the maximum index count {}", desc.count, m_DeviceLimits->getMaxIndexCount());
    }

    IndexBuffer::~IndexBuffer() = default;

    u64 IndexBuffer::getCount() const {
        return m_Count;
    }

    SR_TYPE IndexBuffer::getDataType() const {
        return m_DataType;
    }
}