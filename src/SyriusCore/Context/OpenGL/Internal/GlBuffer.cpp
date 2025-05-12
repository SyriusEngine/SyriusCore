#include "GlBuffer.hpp"

namespace Syrius {

    GlBuffer::GlBuffer(const std::string &name, GLenum target, Size size, const void *data, SR_BUFFER_USAGE usage):
    m_Name(name),
    m_Size(size),
    m_Target(target),
    m_Usage(usage){
        glCreateBuffers(1, &m_BufferID);
        glNamedBufferData(m_BufferID, size, data, getGlBufferType(usage));

        SR_POSTCONDITION(m_BufferID != 0, "[{}]: Buffer object not created!", m_Name);
    }

    GlBuffer::~GlBuffer() {
        release();
    }

    void GlBuffer::release() {
        if (m_BufferID != 0) {
            glDeleteBuffers(1, &m_BufferID);
            m_BufferID = 0;
        }

        SR_POSTCONDITION(m_BufferID == 0, "[{}]: Buffer object not deleted!", m_Name);
    }

    void GlBuffer::bind() const {
        SR_PRECONDITION(m_BufferID != 0, "[{}]: Buffer not created!", m_Name);
        SR_PRECONDITION(m_Target != 0, "[{}]: Target not set!", m_Name);

        glBindBuffer(m_Target, m_BufferID);
    }

    void GlBuffer::bindShaderResource(const u32 slot) const {
        SR_PRECONDITION(m_BufferID != 0, "[{}]: Buffer not created!", m_Name);
        SR_PRECONDITION(m_Target != 0, "[{}]: Target not set!", m_Name);

        glBindBufferBase(m_Target, slot, m_BufferID);
    }

    void GlBuffer::setData(const void *data, Size size) {
        SR_PRECONDITION(m_BufferID != 0, "[{}]: Buffer not created!", m_Name);
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[{}]: Update on buffer object {} requested, which was created with {} usage flag!", m_Name, m_BufferID, m_Usage);

        // 1. Check Sizes
        if (size > m_Size) {
            SR_LOG_WARNING(m_Name, "Data size {} exceeds the current buffer size {}. Data will be partially written!", size, m_Size);
        }
        const Size copySize = std::min(m_Size, size);

        // 2. Map the buffer
        void* pBuffer = glMapNamedBuffer(m_BufferID, GL_WRITE_ONLY);
        if (!pBuffer) {
            SR_LOG_WARNING(m_Name, "Failed to map buffer object {}", m_BufferID);
        }

        // 3. Copy data
        memcpy(pBuffer, data, copySize);

        // 4. Unmap the buffer
        const GLboolean result = glUnmapNamedBuffer(m_BufferID);
        if (result == GL_FALSE) {
            SR_LOG_WARNING(m_Name, "Failed to unmap buffer object {}", m_BufferID);
        }
    }

    void GlBuffer::copyFrom(const GlBuffer &other) {
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[{}]: Update on buffer object {} requested, which was created with {} usage flag!", m_Name, m_BufferID, m_Usage);

        if (m_Size < other.m_Size) {
            SR_LOG_WARNING(m_Name, "Copy from buffer object {} requested, which exceeds the current buffer size ({} > {}). Data will be partially copied", other.m_BufferID, other.m_Size, m_Size);
        }
        const Size copySize = std::min(m_Size, other.m_Size);
        glCopyNamedBufferSubData(other.m_BufferID, m_BufferID, 0, 0, copySize);
    }

    UP<UByte[]> GlBuffer::getData() const {
        auto data = createUP<UByte[]>(m_Size);
        void* pBuffer = glMapNamedBuffer(m_BufferID, GL_READ_ONLY);
        if (!pBuffer) {
            SR_LOG_WARNING(m_Name, "Failed to map buffer object {}", m_BufferID);
            return std::move(data);
        }

        memcpy(data.get(), pBuffer, m_Size);
        const GLboolean result = glUnmapNamedBuffer(m_BufferID);
        if (result == GL_FALSE) {
            SR_LOG_WARNING(m_Name, "Failed to unmap buffer object {}", m_BufferID);
        }
        return std::move(data);
    }

    GLuint GlBuffer::getBufferID() const {
        return m_BufferID;
    }

    Size GlBuffer::getSize() const {
        return m_Size;
    }

    SR_BUFFER_USAGE GlBuffer::getUsage() const {
        return m_Usage;
    }

    const std::string& GlBuffer::getName() const {
        return m_Name;
    }

    GLenum GlBuffer::getTarget() const {
        return m_Target;
    }

}
