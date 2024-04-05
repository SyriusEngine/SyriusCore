#include "../../../include/SyriusCore/Context/IndexBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    IndexBuffer::IndexBuffer(const IndexBufferDesc &desc, const Resource<DeviceLimits>& deviceLimits):
            m_DeviceLimits(deviceLimits),
            m_Count(desc.count),
            m_Usage(desc.usage),
            m_DataType(desc.dataType) {
        SR_CORE_PRECONDITION(desc.dataType >= SR_UINT8 and desc.dataType <= SR_INT32, "[IndexBuffer]: Index buffer only supports integer data, type %i is not supported", desc.dataType);
        SR_CORE_PRECONDITION(desc.count > 0, "[IndexBuffer]: Index buffer count must be greater than %i", 0);
        SR_CORE_PRECONDITION(desc.count <= m_DeviceLimits->getMaxIndexCount(), "[IndexBuffer]: Supplied index count (%i) exceeds the maximum index count (%i)", desc.count, m_DeviceLimits->getMaxIndexCount());

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

    SR_BUFFER_USAGE IndexBuffer::getUsage() const {
        return m_Usage;
    }

    SR_TYPE IndexBuffer::getDataType() const {
        return m_DataType;
    }
}