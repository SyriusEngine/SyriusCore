#include "GlConstantBuffer.hpp"

namespace Syrius{

    GlConstantBuffer::GlConstantBuffer(const ConstantBufferDesc &desc)
    : ConstantBuffer(desc),
    m_BufferID(0){
        glCreateBuffers(1, &m_BufferID);
        glNamedBufferData(m_BufferID, m_Size, desc.data, getGlBufferType(desc.type));
    }

    GlConstantBuffer::~GlConstantBuffer() {
        glDeleteBuffers(1, &m_BufferID);
    }

    void GlConstantBuffer::bind() {
        glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingIndex, m_BufferID);
    }

    void GlConstantBuffer::setData(const void *data) {
        glNamedBufferSubData(m_BufferID, 0, m_Size, data);
    }

    void GlConstantBuffer::addShader(Shader *shader) {
        auto shaderID = shader->getIdentifier();
        auto index = glGetUniformBlockIndex(shaderID, m_BlockName.c_str());
        glUniformBlockBinding(shaderID, index, m_BindingIndex);
    }

    uint64 GlConstantBuffer::getIdentifier() const {
        return m_BufferID;
    }
}