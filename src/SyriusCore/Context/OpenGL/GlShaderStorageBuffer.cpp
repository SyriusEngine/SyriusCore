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
        SR_CORE_PRECONDITION(size <= m_Size, "[ShaderStorageBuffer]: supplied size (%i) exceeds the buffer size (%i)", size, m_Size);

        glNamedBufferSubData(m_BufferID, 0, size, data);
    }

    Resource<byte[]> GlShaderStorageBuffer::getData() const {
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT); // ensure that all writing operations are done before trying to read the data from this buffer

        Resource<byte[]> data = createResource<byte[]>(m_Size);

        auto ptr = glMapNamedBuffer(m_BufferID, GL_READ_ONLY);
        memcpy(data.get(), ptr, m_Size);
        glUnmapNamedBuffer(m_BufferID);
        return std::move(data);
    }

    uint64 GlShaderStorageBuffer::getIdentifier() const {
        return m_BufferID;
    }
}