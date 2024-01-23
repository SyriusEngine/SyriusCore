#include "../../../include/SyriusCore/Context/ShaderStorageBuffer.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ShaderStorageBuffer::ShaderStorageBuffer(const ShaderStorageBufferDesc &desc, const Resource<DeviceLimits> &deviceLimits):
    m_DeviceLimits(deviceLimits),
    m_Name(desc.name),
    m_Size(desc.size),
    m_Usage(desc.usage){
        SR_CORE_PRECONDITION(desc.size > 0, "[ShaderStorageBuffer]: Buffer %s size must be greater than 0", desc.name.c_str());

    }

    ShaderStorageBuffer::~ShaderStorageBuffer() {

    }

    const std::string &ShaderStorageBuffer::getName() const {
        return m_Name;
    }

    uint32 ShaderStorageBuffer::getSize() const {
        return m_Size;
    }
}
