#include "GlShaderStorageBuffer.hpp"

namespace Syrius{

    GlShaderStorageBuffer::GlShaderStorageBuffer(const ShaderStorageBufferDesc &desc, const Resource<DeviceLimits> &deviceLimits) :
    ShaderStorageBuffer(desc, deviceLimits),
    m_BufferID(0){
        glCreateBuffers(1, &m_BufferID);
        glNamedBufferData(m_BufferID, m_Size, desc.data, getGlBufferUsage(desc.usage));

        SR_CORE_POSTCONDITION(m_BufferID != 0, "[ShaderStorageBuffer]: failed to create buffer: %s", desc.name.c_str());
    }

    GlShaderStorageBuffer::~GlShaderStorageBuffer() {
        glDeleteBuffers(1, &m_BufferID);
    }

    void GlShaderStorageBuffer::bind() {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);
    }

    void GlShaderStorageBuffer::bindShaderResource(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxShaderStorageBufferSlots(), "[ShaderStorageBuffer]: supplied slot (%i) exceeds the maximum number of shader storage buffer slots (%i)", slot, m_DeviceLimits->getMaxShaderStorageBufferSlots());

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_BufferID);
    }

    void GlShaderStorageBuffer::setData(const void *data, uint32 size) {
        SR_CORE_PRECONDITION(m_Usage == SR_BUFFER_USAGE_DYNAMIC, "[ShaderStorageBuffer]: Update on buffer object (%p) requested, which has not been created with SR_BUFFER_USAGE_DYNAMIC flag!", this);
        SR_CORE_PRECONDITION(size <= m_Size, "[ShaderStorageBuffer]: Update on buffer object (%p) requested, which exceeds the current buffer size (%i > %i).", this, size, m_Size);

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_WRITE_ONLY);
        if (!pBuffer){
            SR_CORE_THROW("[GlShaderStorageBuffer]: Failed to map buffer object (%i)", m_BufferID);
        }
        memcpy(pBuffer, data, size);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        SR_CORE_ASSERT(retVal, "[GlShaderStorageBuffer]: Failed to unmap buffer object (%i)", m_BufferID);
    }

    Resource<ubyte[]> GlShaderStorageBuffer::getData() const {
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT); // ensure that all writing operations are done before trying to read the data from this buffer

        auto data = createResource<ubyte[]>(m_Size);
        auto pBuffer = glMapNamedBuffer(m_BufferID, GL_READ_ONLY);
        if (!pBuffer){
            SR_CORE_THROW("[GlShaderStorageBuffer]: Failed to map buffer object (%i)", m_BufferID);
        }

        memcpy(data.get(), pBuffer, m_Size);
        auto retVal = glUnmapNamedBuffer(m_BufferID);
        SR_CORE_ASSERT(retVal, "[GlShaderStorageBuffer]: Failed to unmap buffer object (%i)", m_BufferID);
        return std::move(data);
    }

    uint64 GlShaderStorageBuffer::getIdentifier() const {
        return m_BufferID;
    }
}